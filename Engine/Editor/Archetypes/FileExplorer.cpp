//
//	FileExplorer.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	namespace {
		constexpr const unsigned char eFolderT = 0, eLevelT = 1, eAudioT = 2, eAnimationT = 3, eUnknownT = 4;
		constexpr const unsigned char eNameS = 0, ePathS = 1, eFolderS = 2, eLevelS = 3, eAudioS = 4, eAnimationS = 5, eUnknownS = 6;
	}

	// ------------------------------------------------------------------------
	/*! Get Files In Path
	*
	*   Get the files listed within a path
	*/ //--------------------------------------------------------------------
	void File::GetFilesIInPath(const path_t& mPath, Engine::Array<File>& files) noexcept {
		files.clear();

		//If the path does not exist
		if (std::filesystem::exists(mPath)) {
			//For every file in directory
			for (const auto& dirEntry : std::filesystem::directory_iterator(mPath)) {
				const std::filesystem::path& _dirPath = dirEntry.path();
				const std::string _metaf = _dirPath.filename().string();
				std::string _extension;
				size_t _idx = _metaf.find_last_of('.');

				//If we have an extension
				if (_idx != std::string::npos)
					_extension = _metaf.substr(_idx + 1);

				//If we have a path instead
				else if (std::filesystem::is_directory(_dirPath)) {
					files.push_back({ _dirPath.filename().string().c_str(), _dirPath,
						File::Filetype::eFolder });
					continue;
				}
				else
					continue;

				//If the pointed file is a level
				if (!strcmp(_extension.c_str(), "level"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eLevel });

				//If the pointer file is a png
				else if (!strcmp(_extension.c_str(), "png") || !strcmp(_extension.c_str(), "jpg"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eTexture });

				//If the pointer file is a png
				else if (!strcmp(_extension.c_str(), "fbx") || !strcmp(_extension.c_str(), "obj") || !strcmp(_extension.c_str(), "dae"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eModel });

				//If the pointed file is a wav file
				else if (!strcmp(_extension.c_str(), "wav") ||
					!strcmp(_extension.c_str(), "mp3"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eAudio });

				//If the pointed file is a space file
				else if (!strcmp(_extension.c_str(), "atlas"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eAnimation });
				else if (!strcmp(_extension.c_str(), "prefab"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::ePrefab });

				//If the pointed file is none of the above
				else
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eUnknown, });
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a Model given a title
	*/ //--------------------------------------------------------------------
	File_browser_modal::File_browser_modal(const char* title, const char* mPath) noexcept :
		mTitle(title), mOldVisibility(false), mSelection(0), mCurrentPath(mPath), mCurrentPathIsDir(true) {
		File::GetFilesIInPath(mCurrentPath, mFilesInScope);
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Render a browser Modal
	*/ //--------------------------------------------------------------------
	const bool File_browser_modal::Present(const bool isVisible, std::string& outPath) noexcept {
		bool _result = false;
		static char _filestring[_MAX_PATH] = { 0 };

		const auto vector_file_items_getter = [](void* data, int idx, const char** out_text) noexcept {
			*out_text = reinterpret_cast<Engine::Array<File>*>(data)->at(idx).mAlias.data();

			return true;
		};

		//If we need to reopen our modal
		if (mOldVisibility != isVisible) {
			mOldVisibility = isVisible;

			//If it's visible
			if (isVisible) {
				mCurrentPath = outPath;
				mCurrentPathIsDir = true;
				ImGui::OpenPopup(mTitle);
			}
		}

		static bool _isOpen = true;

		//If we can open our modal
		if (ImGui::BeginPopupModal(mTitle, &_isOpen, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_AlwaysAutoResize)) {
			//Show the List of files
			if (ImGui::ListBox("##", &mSelection, vector_file_items_getter, &mFilesInScope,
				static_cast<int>(mFilesInScope.size()), 10)) {
				mCurrentPath = mFilesInScope[mSelection].mPath;
				mCurrentPathIsDir = std::filesystem::is_directory(mCurrentPath);

				//If the selection is a directory, repopulate the list with the contents of that directory.
				if (mCurrentPathIsDir)
					File::GetFilesIInPath(mCurrentPath, mFilesInScope);
			}

			char _buff[_MAX_PATH] = { 0 };

			ImGui::PushItemWidth(-1);
			ImGui::InputText(_buff, _buff, _MAX_PATH);

			//If we have a valid string
			if (*_buff != '\0') strcpy_s(_filestring, _buff);

			ImGui::PopItemWidth();
			ImGui::Spacing();
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);

			// Make the "Select" button look / act disabled if the current selection is a directory.
			if (*_filestring == '\0') {
				static const ImVec4 _disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

				ImGui::PushStyleColor(ImGuiCol_Button, _disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, _disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _disabledColor);
				ImGui::Button("Save");
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}
			else {
				//If we pressed Save
				if (ImGui::Button("Save")) {
					ImGui::CloseCurrentPopup();

					//If we have a Directory
					if (mCurrentPathIsDir)
						outPath = mCurrentPath.string() + std::string("/") + std::string(_filestring);
					else
						outPath = mCurrentPath.string();

					_result = true;
				}
			}

			ImGui::EndPopup();
		}

		_isOpen = true;

		return _result;
	}
}

#endif