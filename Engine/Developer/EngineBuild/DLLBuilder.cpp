#include <Shared.h>

namespace Engine {
	Bottler::Process::Process(const string_type& command, const string_type& mPath,
		read_fn read_stdout,
		read_fn read_stderr,
		bool open_stdin) noexcept
		: closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin) {
		open(command, mPath);
		async_read();
	}

	Bottler::Process::~Process() noexcept {
		close_fds();
	}

	Bottler::Process::Data::Data() noexcept :
		id(NULL) {
	}

	std::mutex create_process_mutex;

	Bottler::Process::id_type Bottler::Process::open(const arguments_type& arguments,
		const string_type& mPath, const environment_type* environment) noexcept {
		string_type command;
		for (auto& argument : arguments)
			command += (command.empty() ? "" : " ") + argument;
		return open(command, mPath, environment);
	}

	Bottler::Process::id_type Bottler::Process::open(const string_type& command,
		const string_type& mPath, const environment_type* environment) noexcept {
		if (open_stdin) stdin_fd = std::unique_ptr<fd_type>(new fd_type(nullptr));
		if (read_stdout) stdout_fd = std::unique_ptr<fd_type>(new fd_type(nullptr));
		if (read_stderr) stderr_fd = std::unique_ptr<fd_type>(new fd_type(nullptr));

		Handle stdin_rd_p;
		Handle stdin_wr_p;
		Handle stdout_rd_p;
		Handle stdout_wr_p;
		Handle stderr_rd_p;
		Handle stderr_wr_p;

		SECURITY_ATTRIBUTES security_attributes;

		security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		security_attributes.bInheritHandle = TRUE;
		security_attributes.lpSecurityDescriptor = nullptr;

		std::lock_guard<std::mutex> lock(create_process_mutex);

		if (stdin_fd) {
			if (!CreatePipe(&stdin_rd_p, &stdin_wr_p, &security_attributes, 0) ||
				!SetHandleInformation(stdin_wr_p, HANDLE_FLAG_INHERIT, 0))
				return 0;
		}

		if (stdout_fd) {
			if (!CreatePipe(&stdout_rd_p, &stdout_wr_p, &security_attributes, 0) ||

				!SetHandleInformation(stdout_rd_p, HANDLE_FLAG_INHERIT, 0)) {
				return 0;
			}
		}

		if (stderr_fd) {
			if (!CreatePipe(&stderr_rd_p, &stderr_wr_p, &security_attributes, 0) ||

				!SetHandleInformation(stderr_rd_p, HANDLE_FLAG_INHERIT, 0)) {
				return 0;
			}
		}

		PROCESS_INFORMATION process_info;
		STARTUPINFO startup_info;

		ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
		ZeroMemory(&startup_info, sizeof(STARTUPINFO));
		startup_info.cb = sizeof(STARTUPINFO);
		startup_info.hStdInput = stdin_rd_p;
		startup_info.hStdOutput = stdout_wr_p;
		startup_info.hStdError = stderr_wr_p;
		if (stdin_fd || stdout_fd || stderr_fd)
			startup_info.dwFlags |= STARTF_USESTDHANDLES;

		auto process_command = command;

		string_type environment_str;
		if (environment) {
			for (const auto& e : *environment)
				environment_str += e.first + '=' + e.second + '\0';
			environment_str += '\0';
		}
		BOOL bSuccess = CreateProcess(nullptr, process_command.empty() ? nullptr : &process_command[0], nullptr, nullptr,
			stdin_fd || stdout_fd || stderr_fd,
			stdin_fd || stdout_fd || stderr_fd ? CREATE_NO_WINDOW : 0,
			environment_str.empty() ? nullptr : &environment_str[0],
			mPath.empty() ? nullptr : mPath.c_str(),
			&startup_info, &process_info);

		if (!bSuccess)
			return 0;
		else
			CloseHandle(process_info.hThread);

		if (stdin_fd) *stdin_fd = stdin_wr_p.detach();
		if (stdout_fd) *stdout_fd = stdout_rd_p.detach();
		if (stderr_fd) *stderr_fd = stderr_rd_p.detach();

		closed = false;
		data.id = process_info.dwProcessId;
		data.handle = process_info.hProcess;
		return process_info.dwProcessId;
	}

	void Bottler::Process::async_read() noexcept {
		if (data.id == 0)
			return;

		if (stdout_fd) {
			stdout_thread = std::thread([this]() {
				DWORD n;
				std::unique_ptr<char[]> buffer(new char[(1 << 17)]);
				for (;;) {
					BOOL bSuccess = ReadFile(*stdout_fd, static_cast<CHAR*>(buffer.get()), static_cast<DWORD>(1 << 17), &n, nullptr);
					if (!bSuccess || n == 0)
						break;
					read_stdout(buffer.get(), static_cast<size_t>(n));
				}
				});
		}
		if (stderr_fd) {
			stderr_thread = std::thread([this]() {
				DWORD n;
				std::unique_ptr<char[]> buffer(new char[(1 << 17)]);
				for (;;) {
					BOOL bSuccess = ReadFile(*stderr_fd, static_cast<CHAR*>(buffer.get()), static_cast<DWORD>(1 << 17), &n, nullptr);
					if (!bSuccess || n == 0)
						break;
					read_stderr(buffer.get(), static_cast<size_t>(n));
				}
				});
		}
	}

	int Bottler::Process::GetExitStatus() noexcept {
		if (data.id == 0)
			return -1;

		if (!data.handle)
			return data.exit_status;

		WaitForSingleObject(data.handle, INFINITE);

		DWORD exit_status;
		if (!GetExitCodeProcess(data.handle, &exit_status))
			data.exit_status = -1; // Store exit status for future calls
		else
			data.exit_status = static_cast<int>(exit_status); // Store exit status for future calls

		{
			std::lock_guard<std::mutex> lock(close_mutex);
			CloseHandle(data.handle);
			data.handle = nullptr;
			closed = true;
		}
		close_fds();

		return data.exit_status;
	}

	void Bottler::Process::close_fds() noexcept {
		if (stdout_thread.joinable())
			stdout_thread.join();
		if (stderr_thread.joinable())
			stderr_thread.join();

		if (stdin_fd)
			close_stdin();
		if (stdout_fd) {
			if (*stdout_fd != nullptr)
				CloseHandle(*stdout_fd);
			stdout_fd.reset();
		}
		if (stderr_fd) {
			if (*stderr_fd != nullptr)
				CloseHandle(*stderr_fd);
			stderr_fd.reset();
		}
	}

	void Bottler::Process::close_stdin() noexcept {
		std::lock_guard<std::mutex> lock(stdin_mutex);
		if (stdin_fd) {
			if (*stdin_fd != nullptr)
				CloseHandle(*stdin_fd);
			stdin_fd.reset();
		}
	}

	static void _handle_output(const char* bytes, size_t n) {
		GConsole->Log(Console::eMessageType::e_Message, "%s", std::string(bytes, n).c_str());
	}

	std::string GetCurrentDirectory() {
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");

		return std::string(buffer).substr(0, pos);
	}

	bool copyFile(const char* SRC, const char* DEST) {
		std::string currentdir = GetCurrentDirectory() + '\\';
		std::ifstream src(currentdir + SRC, std::ios::binary);
		std::ofstream dest(currentdir + DEST, std::ios::binary);
		dest << src.rdbuf();
		return src && dest;
	}

	Bottler::Bottler()
		: pluginmodule(nullptr){}

	bool Bottler::Bottle(const char* path) {

		std::string name = path;
		name.resize(name.size() - 4);
		
		TCHAR szFileName[MAX_PATH];
		GetModuleFileName(NULL, szFileName, MAX_PATH);
		
		std::string exepath(szFileName);
		exepath.resize(exepath.size() - 10);
		std::string newdll = std::string(name + "_.dll");

		mPath = exepath + newdll;

		CopyFile((exepath + path).c_str(), mPath.c_str(), false);

		if (pluginmodule = LoadLibrary(newdll.c_str()))
			return true;

		return false;
	}

	void Bottler::Unbottle() {
		FreeLibrary(pluginmodule);
		DeleteFile(mPath.c_str());
	}

	void Bottler::CompileSource() {
		Unbottle();
		std::thread fermentation([this] {
			std::string buildoption = "msbuild \"..\\Good Neighbours.sln\" -target:Source";

			compiler_process = std::unique_ptr<Process>(new
				Process(buildoption, "",
					_handle_output, _handle_output));
			auto exit_status = compiler_process.get()->GetExitStatus();
			GConsole->Log(Console::eMessageType::e_Message, "Process Exited with code: %s", exit_status == 0 ? "Success" : "Failure");

			if (!exit_status) {
				nlohmann::json mTemporalScene;
				GWorld->GetScene()->ToJson(mTemporalScene);
				GWorld->GetScene()->Clear();
				GFactory->ClearCreators();
				Bottle("Source.dll");
				GWorld->GetScene()->FromJson(mTemporalScene);
			}
			});

		fermentation.detach();
	}

	void Bottler::ReloadSource() {
		GFactory->ClearCreators();
		Unbottle();
		GConsole->Log(Console::eMessageType::e_Message, "Source Detached");
		Bottle("Source.dll");
		GConsole->Log(Console::eMessageType::e_Message, "New Source Attached");
	}
}