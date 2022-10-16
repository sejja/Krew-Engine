#ifndef _DLL_BUILDER__H_
#define _DLL_BUILDER__H_

namespace Engine {
	class Bottler : public Singleton<Bottler> {
		template<typename T>
		friend class Singleton;
		
		Bottler();

	public:
		bool Bottle(const char* mPath);
		void Unbottle();
		void CompileSource();
		void ReloadSource();

		struct Process {
		public:
			using id_type = unsigned long;
			using fd_type = void*;
			using string_type = std::string;
			using arguments_type = std::vector<string_type>;
			using read_fn = std::function<void(const char* bytes, size_t n)>;
			using read_fn_ref = std::function<void(const char* bytes, size_t n)>;
			using environment_type = std::unordered_map<string_type, string_type>;

		private:
			class Data {
			public:
				Data() noexcept;
				id_type id;
				HANDLE handle{ nullptr };
				int exit_status{ -1 };
			};

			class Handle {
			public:
				Handle() noexcept : handle(INVALID_HANDLE_VALUE) {}
				~Handle() noexcept {
					close();
				}
				void close() noexcept {
					if (handle != INVALID_HANDLE_VALUE)
						CloseHandle(handle);
				}
				HANDLE detach() noexcept {
					HANDLE old_handle = handle;
					handle = INVALID_HANDLE_VALUE;
					return old_handle;
				}
				operator HANDLE() const noexcept { return handle; }
				HANDLE* operator&() noexcept { return &handle; }

			private:
				HANDLE handle;
			};

		public:
			Process(const string_type& command, const string_type& mPath = string_type(),
				read_fn read_stdout = nullptr, read_fn read_stderr = nullptr,
				bool open_stdin = false) noexcept;
			~Process() noexcept;
			int GetExitStatus() noexcept;

		private:
			Data data;
			bool closed;
			std::mutex close_mutex;
			read_fn_ref read_stdout;
			read_fn_ref read_stderr;
			std::thread stdout_thread, stderr_thread;
			bool open_stdin;
			std::mutex stdin_mutex;

			std::unique_ptr<fd_type> stdout_fd, stderr_fd, stdin_fd;

			id_type open(const arguments_type& arguments, const string_type& mPath,
				const environment_type* environment = nullptr) noexcept;
			id_type open(const string_type& command, const string_type& mPath,
				const environment_type* environment = nullptr) noexcept;
			void async_read() noexcept;
			void close_fds() noexcept;
			void close_stdin() noexcept;
		};

	protected:
		std::string mPath;
		HMODULE pluginmodule;
		std::unique_ptr<Process> compiler_process;
		nlohmann::json j_;
	};
}

#define GBinaryLoader (&Engine::Bottler::Instance())

#endif