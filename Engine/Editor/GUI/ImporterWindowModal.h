//
//  ImporterWindowModal.h
//  Good Neighbours
//
//  Created by Diego Revilla 18/09/21.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifdef _EDITOR_

#ifndef _IMPORTER_WINDOW_MODAL__H_
#define _IMPORTER_WINDOW_MODAL__H_

namespace Editor {
	class ImporterWindowModal : public IWindow {
#pragma region // Functions
	public:
		ImporterWindowModal() noexcept;
		void Present() noexcept override;
#pragma endregion

#pragma region // Members
	private:
		File_browser_modal mFileBrowser;
#pragma endregion
	};

	class PrefabWindowModal : public IWindow {
#pragma region // Functions
	public:
		PrefabWindowModal() noexcept;
		void Present() noexcept override;
#pragma endregion

#pragma region // Members
	private:
		File_browser_modal mFileBrowser;
#pragma endregion
	};

	class LevelRetrievalModal : public IWindow {
#pragma region // Functions
	public:
		LevelRetrievalModal() noexcept;
		void Present() noexcept override;
#pragma endregion
	private:
		std::string mTitle;
		bool mOldVisibility;
	};

}

#endif
#endif