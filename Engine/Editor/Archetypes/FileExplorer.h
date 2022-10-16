//
//	FileExplorer.h
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _FILE_EXPLORER__H_
#define _FILE_EXPLORER__H_

namespace Editor {
	struct File {
#pragma region // Declarations
		enum class Filetype : unsigned char {
			eFolder,
			eLevel,
			eTexture,
			eAudio,
			eAnimation,
			eModel,
			ePrefab,
			eUnknown
		};

		using path_t = std::filesystem::path;
#pragma endregion

#pragma region // Members
		static void GetFilesIInPath(const path_t& patjh, Engine::Array<File>& files) noexcept;
		std::string mAlias;
		path_t mPath;
		Filetype mFiletype;
		GLuint GetHandleFromFile() const noexcept;
#pragma endregion
	};

	class File_browser_modal {
#pragma region // Functions
	public:
		explicit File_browser_modal(const char* title, const char* mPath) noexcept;
		const bool Present(const bool isVisible, std::string& outPath) noexcept;
#pragma endregion

#pragma region // Members
	private:
		const char* mTitle;
		bool mOldVisibility;
		int mSelection;
		std::filesystem::path mCurrentPath;
		bool mCurrentPathIsDir;
		Engine::Array<File> mFilesInScope;
#pragma endregion
	};
}

#endif
#endif