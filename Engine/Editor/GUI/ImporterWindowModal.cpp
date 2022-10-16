//
//  ImporterWindowModal.cpp
//  Good Neighbours
//
//  Created by Diego Revilla 18/09/21.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs an Importer Modal Window and sets the default path
	*/ // ---------------------------------------------------------------------
	ImporterWindowModal::ImporterWindowModal() noexcept :
		mFileBrowser("Save to", "EngineContent/") {}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Importer Modal
	*/ // ---------------------------------------------------------------------
	void ImporterWindowModal::Present() noexcept {
		std::string _temp = "EngineContent";

		//If we enter a valid file
		if (mFileBrowser.Present(true, _temp))
		{
			nlohmann::json _outjson;
			GWorld->GetScene()->ToJson(_outjson);
			std::ofstream _output(_temp + ".level");
			_output << _outjson;
			_output.close(), GetOwner()->RemoveWindow(this);
			reinterpret_cast<MainState*>(GetOwner())->mCurrentLevel = _temp + ".level";
		}
	}

	PrefabWindowModal::PrefabWindowModal() noexcept :
		mFileBrowser("Save to", "EngineContent/Prefabs/")
	{

	}
	void PrefabWindowModal::Present() noexcept
	{
		std::string _temp = "EngineContent/Prefabs/";
		if (mFileBrowser.Present(true, _temp))
		{
			nlohmann::json _outjson;

			GEditor->GetSelectedObject().front()->mType = _temp;
			GEditor->GetSelectedObject().front()->ToJson(_outjson);

			std::ofstream _output(_temp + ".prefab");
			_output << _outjson;
			_output.close(), GetOwner()->RemoveWindow(this);
		}
	}

	LevelRetrievalModal::LevelRetrievalModal() noexcept :
		mTitle("Backup Detected"), mOldVisibility(false) {

	}

	void LevelRetrievalModal::Present() noexcept
	{
		ImGui::OpenPopup(mTitle.c_str());

		static bool _isOpen = true;

		//If we can open our modal
		if (ImGui::BeginPopupModal(mTitle.c_str(), &_isOpen, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_AlwaysAutoResize)) {

			ImGui::Text("A previous un-saved backup has been detected. Do you wish to restore it?");

			if (ImGui::Button("Restore")) {

				CHAR my_documents[MAX_PATH];
				HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

				if (result == S_OK) {
					std::string path = std::string(my_documents) + "\\Backup.level";
					nlohmann::json j_;
					std::ifstream input(path);
					try {
						input >> j_;
					}
					catch (...) {}
					input.close();
					GWorld->GetScene()->FromJson(j_);
					reinterpret_cast<MainState*>(GetOwner())->mCurrentLevel = path;
				}
				ImGui::CloseCurrentPopup();
				GetOwner()->RemoveWindow(this);
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
				GetOwner()->RemoveWindow(this);
			}

			ImGui::EndPopup();
		}

		_isOpen = true;
	}
}
#endif