#ifndef _FILE_PATH__H_
#define _FILE_PATH__H_

namespace Engine {
	namespace Platform {
		namespace FileSystem {
			struct FilePath {
				std::string directory;
				std::string extension;
				std::string filename;
				std::string fullPath;

				FilePath();
				FilePath(const char* mPath);
				void ParsePath(const char* mPath = nullptr);
				size_t ListFiles(std::list<std::string>& outList,
					const char* searchPattern = "*.*",
					bool recursive = false) const;

				// ------------------------------------------------------------------------
				/*! Is Folder
				*
				*   Returns wether the Directory is a folder or not
				*/ // --------------------------------------------------------------------
				DONTDISCARD bool inline IsFolder() const {
					return filename.empty() && !directory.empty();
				}

				DONTDISCARD std::string GetCurrentDir();
				bool Copy(const char* src_path,
					const char* dst_path, bool fail_if_exists) const;
				bool Delete(const char* mPath) const;
				bool CreateDir(const char* mPath) const;
				size_t ReadAllFile(const char* filename, char*& out) const;
				DONTDISCARD static std::string GetMyDocuments();
			};
		}
	}
}

#endif