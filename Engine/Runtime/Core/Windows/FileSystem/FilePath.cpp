#include <Shared.h>

namespace Engine {
	namespace Platform {
		namespace FileSystem {
			static constexpr size_t BAD_FILE = -1;

			static const char* common_char_str_[2] = { "delete directory:: delete file",
				"CopyDirectory: Couldn't copy file %s\n\t%s\n\t%s\n" };

			// ------------------------------------------------------------------------
			/*! Copy File Error String
			*
			*   Gets a string of the error thrown by the file system
			*/ // --------------------------------------------------------------------
			static bool CopyFileErrorStr(const char* src_path, const char* dst_path,
				const bool fail_if_exists = false) {
				//If we are failing to copy the file
				if (!::CopyFile(src_path, dst_path, fail_if_exists)) {
					//Switch by the last error
					switch (::GetLastError()) {
						//Case access denied
					case ERROR_ACCESS_DENIED:
						::MessageBoxA(::GetActiveWindow(), common_char_str_[1],
							"ERROR_ACCESS_DENIED", MB_OK);
						break;

						//case encryption failed
					case ERROR_ENCRYPTION_FAILED:
						::MessageBoxA(::GetActiveWindow(), common_char_str_[1],
							"ERROR_ENCRYPTION_FAILED", MB_OK);
						break;

						//any other error
					default:
						::MessageBoxA(::GetActiveWindow(), common_char_str_[1],
							"UNKNOWN_ERROR", MB_OK);
					}

					return false;
				}

				return true;
			}

			// ------------------------------------------------------------------------
			/*! Copy Directory
			*
			*   Duplicates a Directory
			*/ // --------------------------------------------------------------------
			static bool CopyDirectory(const char* src_path, const char* dst_path,
				const bool recursive, bool fail_if_exists) {
				FilePath dst_fp_ = FilePath(dst_path);

				// get the list of the files.
				std::list<std::string> outFiles_;
				FilePath(src_path).ListFiles(outFiles_);

				std::for_each(outFiles_.begin(), outFiles_.end(), [recursive, fail_if_exists, dst_fp_](std::string& x) {
					FilePath fp_(x.c_str());

					//If the source is a folder
					if (fp_.IsFolder()) {
						auto dir = std::move(fp_.directory.substr(0, fp_.directory.length() - 1));
						auto pos = dir.find_last_of("\\");

						//If we are not out of bounds on the directory search
						if (pos == std::string::npos)
							pos = dir.find_last_of("/");

						const std::string new_sub_path = dst_fp_.directory +
							dir.substr(pos + 1) + "/";

						//If it's recursive
						if (recursive)
							CopyDirectory(fp_.fullPath.c_str(), new_sub_path.c_str(),
								recursive, fail_if_exists);
					}
					else

						CopyFileErrorStr(x.c_str(), (dst_fp_.directory +
							fp_.filename + fp_.extension).c_str(),
							fail_if_exists);
					});

				return true;
			}

			// ------------------------------------------------------------------------
			/*! Default Constructor
			*
			*   Constructs a File Path
			*/ // --------------------------------------------------------------------
			FilePath::FilePath() {
			}

			// ------------------------------------------------------------------------
			/*! Custom Constructor
			*
			*   Constructs a File Path given a filepath
			*/ // --------------------------------------------------------------------
			FilePath::FilePath(const char* mPath) {
				ParsePath(mPath);
			}

			// ------------------------------------------------------------------------
			/*! Parse Path
			*
			*   Gets the Path to a file
			*/ // --------------------------------------------------------------------
			void FilePath::ParsePath(const char* mPath) {
				if (mPath != nullptr)
					fullPath = mPath;

				// safeguard
				if (fullPath.length()) {
					std::size_t dir_offset_ = fullPath.find_last_of("\\");
					directory = std::move(fullPath.substr(0, dir_offset_ + 1));
					std::size_t ext_offset = fullPath.find_last_of(".");

					//If we could find the extension
					if (ext_offset != std::string::npos) {
						extension = std::move(fullPath.substr(ext_offset));
						filename = std::move(fullPath.substr(dir_offset_ + 1, ext_offset - dir_offset_ - 1));
					}
					else
						filename = std::move(fullPath.substr(dir_offset_ + 1));
				}
			}

			// ------------------------------------------------------------------------
			/*! Get All Files From Directory Recursive
			*
			*   Gets all the files in a folder and (if wished) all the files within
			*		recursive folders
			*/ // --------------------------------------------------------------------
			static void GetAllFilesFromDirectoryRec(const char* dir,
				std::list<std::string>& outList, const char* pattern,
				bool recursive) {
				//If we have a valid directory
				if (dir) {
					WIN32_FIND_DATA ffd_;
					HANDLE hFind_ = INVALID_HANDLE_VALUE;
					DWORD dwError_ = 0;
					std::string patternStr_;

					//If we have a valid patern
					if (pattern)
						patternStr_ = pattern;

					std::string find_ = dir;
					find_ += "*";
					hFind_ = FindFirstFile(find_.c_str(), &ffd_);

					//If we got an invalid handle (weird)
					if (INVALID_HANDLE_VALUE == hFind_)
						return;

					//do for the fist file
					do {
						std::string tmp = dir;
						tmp += ffd_.cFileName;

						//If we encountered a hiden file
						if (ffd_.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
							continue;

						const std::string fn = ffd_.cFileName;

						//If we encountered return directories
						if (fn == "." || fn == "..")
							continue;

						//If we encountered another folder
						if (ffd_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
							tmp += "/";

							//And we are getting recursive
							if (recursive)
								GetAllFilesFromDirectoryRec(tmp.c_str(), outList, pattern, recursive);
						}

						//If we have a non valid file
						if (patternStr_.empty() ||
							patternStr_ == "*.*" ||
							patternStr_ == "*")
							outList.push_back(tmp);
						else {
							std::size_t star_pos = patternStr_.find_first_of("*");

							//Remove all the directories from the file name
							while (star_pos != std::string::npos) {
								patternStr_.erase(star_pos, 1);
								star_pos = patternStr_.find_first_of("*");
							}

							//If we found a valid name
							if (tmp.find(patternStr_) != std::string::npos)
								outList.push_back(tmp);
						}

						//Do while there are files in the folder
					} while (FindNextFile(hFind_, &ffd_));

					dwError_ = GetLastError();

					//If there are still files
					if (dwError_ != ERROR_NO_MORE_FILES)
						MessageBoxA(::GetActiveWindow(), "Files out of reach",
							"Please, revise include directories", MB_OK);

					FindClose(hFind_);
				}
			}

			using namespace FileSystem;

			// ------------------------------------------------------------------------
			/*! List Files
			*
			*   List all files within a folder
			*/ // --------------------------------------------------------------------
			size_t FilePath::ListFiles(std::list<std::string>& outList,
				const char* searchPattern, const bool recursive) const {
				GetAllFilesFromDirectoryRec(directory.c_str(),
					outList, searchPattern, recursive);

				return outList.size() - outList.size();
			}

			// ------------------------------------------------------------------------
			/*! Get Current Directory
			*
			*   Returns the Current Directory
			*/ // --------------------------------------------------------------------
			std::string FilePath::GetCurrentDir() {
				char buffer[MAX_PATH];
				::GetCurrentDirectory(MAX_PATH, buffer);
				return buffer;
			}

			// ------------------------------------------------------------------------
			/*! Copy
			*
			*   Copies a file or directory
			*/ // --------------------------------------------------------------------
			bool FilePath::Copy(const char* src_path, const char* dst_path,
				const bool fail_if_exists) const {
				//check whether it's a file or a directory
				if (FilePath(src_path).IsFolder())
					return CopyDirectory(src_path, dst_path, true, fail_if_exists);
				else
					return CopyFileErrorStr(src_path, dst_path, fail_if_exists);
			}

			// ------------------------------------------------------------------------
			/*! Delete Directory
			*
			*   Deletes an entire directory
			*/ // --------------------------------------------------------------------
			static bool DeleteDirectory(const char* mPath) {
				std::list<std::string> outVec_;
				FilePath(mPath).ListFiles(outVec_);

				// delete each file in the folder
				for (auto& x : outVec_) {
					const char* fileStr = x.c_str();

					// recursively delete the all the folders
					if (FilePath(fileStr).IsFolder()) {
						// delete the directory recursively
						if (!DeleteDirectory(fileStr))
							return false;
					}
					else

						// delete the file
						if (!::DeleteFile(fileStr)) {
							return false;
						}
				}

				return true;
			}

			// ------------------------------------------------------------------------
			/*! Delete
			*
			*   Deletes a file
			*/ // --------------------------------------------------------------------
			bool FilePath::Delete(const char* mPath) const {
				//But it's folder
				if (FilePath(mPath).IsFolder()) {
					//If we could succesfully delete it
					if (!DeleteDirectory(mPath))
						return false;
				}

				return true;
			}

			// ------------------------------------------------------------------------
			/*! Create Directory
			*
			*   Creates a new Directory
			*/ // --------------------------------------------------------------------
			bool FilePath::CreateDir(const char* mPath) const {
				::CreateDirectory(mPath, 0);

				return true;
			}

			// ------------------------------------------------------------------------
			/*! Read All File
			*
			*   Reads all the contents of one file
			*/ // --------------------------------------------------------------------
			size_t FilePath::ReadAllFile(const char* filename,
				char*& out) const {
				std::ifstream f_(filename);

				//If we can open the file
				if (f_.is_open() && f_.good()) {
					f_.seekg(0, std::ios_base::beg);
					f_.seekg(0, std::ios_base::end);

					size_t size = static_cast<int>(f_.tellg());

					f_.seekg(0, std::ios_base::beg);

					//If we have a valid size
					if (size) {
						out = reinterpret_cast<char*>(
							malloc((size + 1) * sizeof(char)));
						f_.read(out, size);
					}

					f_.close();

					return size;
				}

				return BAD_FILE;
			}

			// ------------------------------------------------------------------------
			/*! Get My Documents
			*
			*   Gets the Path to the documents folder
			*/ // --------------------------------------------------------------------
			std::string FilePath::GetMyDocuments() {
				std::string resPathStr_;
				PWSTR resPath_ = NULL;

				size_t strSize_ = (wcslen(resPath_) + 1) * 2;
				char* tmp_ = reinterpret_cast<char*>(malloc(strSize_ * sizeof(char)));
				size_t convertedChars_ = 0;

				if (tmp_) {
					wcstombs_s(&convertedChars_, tmp_, strSize_, resPath_, _TRUNCATE);
					resPathStr_ = tmp_;
					resPathStr_ += "\\";
					free(tmp_);
					CoTaskMemFree(resPath_);
				}

				return resPathStr_;
			}
		}
	}
}