//
//	ContentBrowser.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 09/25/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	namespace {
		Asset<Engine::Graphics::Texture> s_Textures[7] = { nullptr };
		constexpr const unsigned char eFolderT = 0, eLevelT = 1, eAudioT = 2, eAnimationT = 3, eUnknownT = 4;

		const char* s_common_strs[] = { "name", "path", "EngineContent/Texture/Folder.png", "EngineContent/Texture/Level.png"
			, "EngineContent/Texture/Audio.png", "EngineContent/Texture/Bone.png", "EngineContent/Texture/Unknown.png" };
		constexpr const unsigned char eNameS = 0, ePathS = 1, eFolderS = 2, eLevelS = 3, eAudioS = 4, eAnimationS = 5, eUnknownS = 6, eMetaS = 7;
	}

	// ------------------------------------------------------------------------
	/*! Get Handle From File
	*
	*   Gets an specific file handle to represent each file type
	*/ //--------------------------------------------------------------------
	unsigned File::GetHandleFromFile() const noexcept {
		//Switch by the file type
		switch (mFiletype) {
			//case folder
		case File::Filetype::eFolder:
			return s_Textures[0]->Get()->GetGLHandle();

			//case Texture
		case File::Filetype::eTexture:
			return GContent->GetResource<Engine::Graphics::Texture>(this->mPath.string().c_str())->Get()->GetGLHandle();

			//case Audio
		case File::Filetype::eAudio:
			return s_Textures[eAudioT]->Get()->GetGLHandle();

			//case Level
		case File::Filetype::eLevel:
			return s_Textures[eLevelT]->Get()->GetGLHandle();

			//case Space
		case File::Filetype::eAnimation:
			return s_Textures[eAnimationT]->Get()->GetGLHandle();

			//case unknown
		default:
			return s_Textures[eUnknownT]->Get()->GetGLHandle();
		}
	}

	// ------------------------------------------------------------------------
	/*! Thumbnail
	*
	*   Draws a Thumbnail on the ImGui Layer
	*/ //--------------------------------------------------------------------
	bool ThumbNail(ImTextureID user_texture_id, const ImVec2& size, const char* label) noexcept {
		ImGuiButtonFlags button_flags = 0;
		button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems) return false;

		const ImGuiStyle& style = GImGui->Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 button_size = size;
		button_size.y += label_size.y * 2;

		const ImVec2 padding = style.FramePadding;
		const ImRect bb(window->DC.CursorPos, { window->DC.CursorPos.x + button_size.x + padding.x * 2, 
			window->DC.CursorPos.y + button_size.y + padding.y * 2 });
		const ImRect image_bb({ window->DC.CursorPos.x + padding.x, window->DC.CursorPos.y + padding.y }, 
			{ window->DC.CursorPos.x + padding.x + size.x, window->DC.CursorPos.y + padding.y + size.y });
		ImGui::ItemSize(bb);
		if (!ImGui::ItemAdd(bb, id)) return false;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);
		const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(bb, id);
		ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
		window->DrawList->AddImage(user_texture_id, image_bb.Min, image_bb.Max, { 0, 1 }, { 1, 0 }, ImGui::GetColorU32({1, 1, 1, 1}));
		ImRect offset = bb;
		offset.Min.y = image_bb.Max.y + padding.y;
		ImGui::RenderTextClipped({ offset.Min.x + style.FramePadding.x, offset.Min.y + style.FramePadding.y },
			{ offset.Max.x - style.FramePadding.x, offset.Max.y - style.FramePadding.y }, label, NULL, & label_size, style.ButtonTextAlign, & offset);
		return pressed;
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents a File Browser and returns wether we select a file or not
	*/ //--------------------------------------------------------------------
	const bool File_browser::Present(std::string& outPath) noexcept {
		//If we could begin a file explorer
		if (ImGui::Begin(mTitle)) {

			ImGui::Spacing; ImGui::SameLine(); 

			const auto _fgreyout = []() {
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			};

			//If we do not have any previous paths
			if (mPrevPaths.empty()) _fgreyout();

			//Previous path
			if (ImGui::Button("<")) {

				if (mPrevPaths.empty()) {
					ImGui::PopStyleVar();
					ImGui::PopItemFlag();
				}

				mNextPaths.push(mCurrentPath);
				mCurrentPath = mPrevPaths.top(); mPrevPaths.pop();
				File::GetFilesIInPath(mCurrentPath, mFilesInScope);
			} else if (mPrevPaths.empty()) {
				ImGui::PopStyleVar();
				ImGui::PopItemFlag();
			}

			ImGui::SameLine();

			//If we do not have future paths
			if (mNextPaths.empty()) _fgreyout();
			
			if (ImGui::Button(">")) {
				if (mNextPaths.empty()) {
					ImGui::PopStyleVar();
					ImGui::PopItemFlag();
				}


				mPrevPaths.push(mCurrentPath);
				mCurrentPath = mNextPaths.top(); mNextPaths.pop();
				File::GetFilesIInPath(mCurrentPath, mFilesInScope);
			} else if (mNextPaths.empty()) {
				ImGui::PopStyleVar();
				ImGui::PopItemFlag();
			}
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow) && ImGui::GetIO().MouseClicked[1])
				ImGui::OpenPopup("File Explorer options");

			if (ImGui::BeginPopup("File Explorer options")) {
				static std::string foldername_;

				ImGui::InputText("", &foldername_);
				if (ImGui::MenuItem("Create Folder")) File::GetFilesIInPath(mCurrentPath, mFilesInScope);
				ImGui::EndPopup();
			}

			mFilesPerRow = static_cast<unsigned char>(ImGui::GetWindowWidth() / (64 * 2));
			unsigned char _n = 0;

			//Iterate through all the files on the scope
			for (size_t i = 0; i < mFilesInScope.size(); i++) {
				auto x = mFilesInScope[i];

				//If we click on any of the thumbnails
				if (ThumbNail(reinterpret_cast<void*>(static_cast<long long>(
					x.GetHandleFromFile())),
					{ 72, 72 }, x.mAlias.c_str())) {

					mCurrentPathIsDir = std::filesystem::is_directory(x.mPath);

					if (mCurrentPathIsDir) {
						while (!mNextPaths.empty()) { mNextPaths.pop();  }
						File::GetFilesIInPath(x.mPath, mFilesInScope);
						mPrevPaths.push(mCurrentPath);
					}
					else {
						outPath = x.mPath.string();
						ImGui::End();

						return true;
					}

					mCurrentPath = x.mPath;
				}

				if (ImGui::BeginDragDropSource()) {

					// Set payload to carry the index of our item (could be anything)
					mHoldedName = std::string(x.mPath.string());
					
					switch (x.mFiletype)
					{

					case File::Filetype::eAudio:
						ImGui::SetDragDropPayload("CONTENT_AUDIO_FILE", &mHoldedName, sizeof(std::string));
						break;

					case File::Filetype::eTexture:
						ImGui::SetDragDropPayload("CONTENT_TEXTURE_FILE", &mHoldedName, sizeof(std::string));
						break;

					case File::Filetype::eModel:
						ImGui::SetDragDropPayload("CONTENT_MODEL_FILE", &mHoldedName, sizeof(std::string));
						break;

					case File::Filetype::ePrefab:
						ImGui::SetDragDropPayload("CONTENT_PREFAB_FILE", &mHoldedName, sizeof(std::string));
						break;

					default:
						break;
					}

					ImGui::Text(x.mPath.string().c_str());
					ImGui::EndDragDropSource();
				}

				//If there is room for more files on this line
				if (_n <= mFilesPerRow) {
					ImGui::SameLine();
					_n++;
				}
				else
					_n = 0;
			}

			ImGui::End();
		}

		return false;
	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a File Browser
	*/ //--------------------------------------------------------------------
	File_browser::File_browser(DiffPtr<const char> title, DiffPtr<const char> mPath) noexcept :
		mTitle(title),
		mSelection(0),
		mCurrentPath(std::string_view(mPath)),
		mCurrentPathIsDir(true) {
		File::GetFilesIInPath(mCurrentPath, mFilesInScope);
		const char** _j = s_common_strs + eFolderS;
		const std::string _enginepath = "";

		//Load all the required Asset browser files
		for (unsigned char i = eFolderS; i <= eUnknownS; i++, _j++)
			s_Textures[i-eFolderS] = GContent->GetResource<Engine::Graphics::Texture>(*_j);
	}

	// ------------------------------------------------------------------------
	/*! Defaiult Constructor
	*
	*   Sets the File Explorer to the right path
	*/ // ---------------------------------------------------------------------
	ExplorerWindow::ExplorerWindow() noexcept :
		mFileBrowser(File_browser("File Explorer", "EngineContent/")) {
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the content explorer
	*/ // ---------------------------------------------------------------------
	void ExplorerWindow::Present() noexcept {
		//If we choose a new path
		if (mFileBrowser.Present(mPath))
		{

			std::filesystem::path filePath = mPath;
			if (filePath.extension() == ".level" && !GEditor->IsPlaying()) {
				//nlohmann::json j_;
				//std::ifstream input(mPath);
				//
				//input >> j_;
				//input.close();
				//GWorld->GetScene()->FromJson(j_);
				GWorld->ChangeLevel(mPath.c_str());
				reinterpret_cast<MainState*>(GetOwner())->mCurrentLevel = mPath;
			}

			if (filePath.extension() == ".prefab" && !GEditor->IsPlaying())
			{

				auto it = Engine::Allocator<PrefabWindow>::New();
				it->SetPath(filePath.string());

				GetOwner()->AddWindow(it);
				
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Get Path
	*
	*   Returns the Path of the Explorer Window
	*/ // ---------------------------------------------------------------------
	std::string ExplorerWindow::GetPath() noexcept {
		return mPath;
	}

	// ------------------------------------------------------------------------
	/*! Get Current File
	*
	*   Gets the File of an Explorer
	*/ // ---------------------------------------------------------------------
	std::string&& ExplorerWindow::GetCurrentFile() noexcept {
		return std::move(mCurrentfile);
	}

	// ------------------------------------------------------------------------
	/*! Set Path
	*
	*   Sets the Path of the explorer
	*/ // ---------------------------------------------------------------------
	void ExplorerWindow::SetPath(const std::string& file) noexcept {
		mPath = file;
	}

	// ------------------------------------------------------------------------
	/*! Set Current File
	*
	*   Sets the File seleted by the Explorer Window
	*/ // ---------------------------------------------------------------------
	void ExplorerWindow::SetCurrentFile(const std::string& file) noexcept {
		mCurrentfile = file;
	}
}

#endif