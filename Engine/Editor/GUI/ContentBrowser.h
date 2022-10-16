//
//	ContentBrowser.h
//	Good Neighbours
//
//	Created by Diego Revilla on 09/25/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_

#ifndef _CONTENT_BROWSER__H_
#define _CONTENT_BROWSER__H_

namespace Editor {

	class File_browser {
#pragma region // Functions
	public:
		explicit File_browser(DiffPtr<const char> title, DiffPtr<const char> mPath = nullptr) noexcept;
		const bool Present(std::string& outPath) noexcept;
#pragma endregion

#pragma region // Members
	private:
		const char* mTitle;
		int mSelection;
		std::filesystem::path mCurrentPath;
		bool mCurrentPathIsDir;
		Engine::Array<File> mFilesInScope;
		unsigned char mFilesPerRow = 9;
		std::stack<std::filesystem::path> mPrevPaths;
		std::stack<std::filesystem::path> mNextPaths;
		std::string mHoldedName;
#pragma endregion
	};

	class ExplorerWindow : public Editor::IWindow {
#pragma region // Constructors & Destructors
	public:
		ExplorerWindow() noexcept;
#pragma endregion

#pragma region // Functions
		void Present() noexcept override;
		std::string GetPath() noexcept;
		std::string&& GetCurrentFile() noexcept;
		void SetPath(const std::string& file) noexcept;
		void SetCurrentFile(const std::string& file) noexcept;
#pragma endregion

#pragma region //Members
	private:
		std::string mPath;
		std::string mCurrentfile;
		File_browser mFileBrowser;
#pragma endregion
	};
}
#endif
#endif