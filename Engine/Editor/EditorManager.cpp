//
//	Editor.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	static ImFont* debugfont;

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs default variables
	*/ // ---------------------------------------------------------------------
	ImGuiManager::ImGuiManager() :
		mPlayFlags{ false, false }, mRenderOption(0), mRenderFlags{ false, false, false } {}

	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes ImGui and the Editor
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::Initialize() {
		using __Texts::Editor::EditorManager::StaticText;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		  // Enable docking (See ImGui Docking tutorial)
		io.Fonts->AddFontFromFileTTF(StaticText::FontPath, 12.f);

		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &font_cfg);

		// Initialize notify
		ImGui::MergeIconsWithLatestFont(16.f, false);
		debugfont = io.Fonts->AddFontFromFileTTF(StaticText::DebugFontPath, 12.f);
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(GWindow->GetHandle());
		ImGui_ImplOpenGL3_Init(StaticText::OpenGLVersion);

		//imGuiIO.Fonts->AddFontFromFileTTF("../data/Fonts/Ruda-Bold.ttf", 15.0f, &config);
		ImGui::GetStyle().FrameRounding = 4.0f;
		ImGui::GetStyle().GrabRounding = 4.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_::ImGuiCol_Text] = { 0.95f, 0.96f, 0.98f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TextDisabled] = { 0.36f, 0.42f, 0.47f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_WindowBg] = { 0.11f, 0.15f, 0.17f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ChildBg] = { 0.15f, 0.18f, 0.22f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_PopupBg] = { 0.08f, 0.08f, 0.08f, 0.94f };
		colors[ImGuiCol_::ImGuiCol_Border] = { 0.08f, 0.10f, 0.12f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
		colors[ImGuiCol_::ImGuiCol_FrameBg] = { 0.20f, 0.25f, 0.29f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_FrameBgHovered] = { 0.12f, 0.20f, 0.28f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_FrameBgActive] = { 0.09f, 0.12f, 0.14f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TitleBg] = { 0.09f, 0.12f, 0.14f, 0.65f };
		colors[ImGuiCol_::ImGuiCol_TitleBgActive] = { 0.08f, 0.10f, 0.12f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TitleBgCollapsed] = { 0.00f, 0.00f, 0.00f, 0.51f };
		colors[ImGuiCol_::ImGuiCol_MenuBarBg] = { 0.15f, 0.18f, 0.22f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ScrollbarBg] = { 0.02f, 0.02f, 0.02f, 0.39f };
		colors[ImGuiCol_::ImGuiCol_ScrollbarGrab] = { 0.20f, 0.25f, 0.29f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ScrollbarGrabHovered] = { 0.18f, 0.22f, 0.25f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ScrollbarGrabActive] = { 0.09f, 0.21f, 0.31f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_CheckMark] = { 0.28f, 0.56f, 1.00f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_SliderGrab] = { 0.28f, 0.56f, 1.00f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_SliderGrabActive] = { 0.37f, 0.61f, 1.00f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_Button] = { 0.20f, 0.25f, 0.29f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ButtonHovered] = { 0.28f, 0.56f, 1.00f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ButtonActive] = { 0.06f, 0.53f, 0.98f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_Header] = { 0.20f, 0.25f, 0.29f, 0.55f };
		colors[ImGuiCol_::ImGuiCol_HeaderHovered] = { 0.26f, 0.59f, 0.98f, 0.80f };
		colors[ImGuiCol_::ImGuiCol_HeaderActive] = { 0.26f, 0.59f, 0.98f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_Separator] = { 0.20f, 0.25f, 0.29f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_SeparatorHovered] = { 0.10f, 0.40f, 0.75f, 0.78f };
		colors[ImGuiCol_::ImGuiCol_SeparatorActive] = { 0.10f, 0.40f, 0.75f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_ResizeGrip] = { 0.26f, 0.59f, 0.98f, 0.25f };
		colors[ImGuiCol_::ImGuiCol_ResizeGripHovered] = { 0.26f, 0.59f, 0.98f, 0.67f };
		colors[ImGuiCol_::ImGuiCol_ResizeGripActive] = { 0.26f, 0.59f, 0.98f, 0.95f };
		colors[ImGuiCol_::ImGuiCol_Tab] = { 0.11f, 0.15f, 0.17f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TabHovered] = { 0.26f, 0.59f, 0.98f, 0.80f };
		colors[ImGuiCol_::ImGuiCol_TabActive] = { 0.20f, 0.25f, 0.29f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TabUnfocused] = { 0.11f, 0.15f, 0.17f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TabUnfocusedActive] = { 0.11f, 0.15f, 0.17f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_TextSelectedBg] = { 0.26f, 0.59f, 0.98f, 0.35f };
		colors[ImGuiCol_::ImGuiCol_DragDropTarget] = { 1.00f, 1.00f, 0.00f, 0.90f };
		colors[ImGuiCol_::ImGuiCol_NavHighlight] = { 0.26f, 0.59f, 0.98f, 1.00f };
		colors[ImGuiCol_::ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_::ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.20f };
		colors[ImGuiCol_::ImGuiCol_ModalWindowDimBg] = { 0.80f, 0.80f, 0.80f, 0.35f };
		mStates.emplace_back(Engine::Allocator<MainState>::New());

	}

	// ------------------------------------------------------------------------
	/*! Shut Down
	*
	*   Destroys ImGui and it's context
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::Shutdown() {
		mStates.clear();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		DeleteFile(TEXT("_lock"));
	}

	// ------------------------------------------------------------------------
	/*! Dockspace
	*
	*   Creates the Dockspace Context
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::Dockspace() const {
		using __Texts::Editor::EditorManager::StaticText;

		static bool truebool = true;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(StaticText::DockspaceID, &truebool, window_flags);
		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID(StaticText::DockspaceID);
		ImGui::DockSpace(dockspace_id, { 0.0f, 0.0f });
	}

	// ------------------------------------------------------------------------
	/*! Show GUI
	*
	*   Presents the Editor ImGui GUI
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::ShowGUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		Dockspace();
		Engine::FForEach(mStates.begin(), mStates.end(), [](Engine::UniquePointer<IEditorState>& state) {state->Present(); });
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar();
		ImGui::End();
		ImGui::End();
		ImGui::EndFrame();

		//If we are rendering onto viewport
		if (!(mPlayFlags.first && !IsViewport())) {
			// Rendering
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	// ------------------------------------------------------------------------
	/*! Show GUI
	*
	*   Presents the Editor ImGui GUI
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::RenderDebugText() {
		using __Texts::Editor::EditorManager::StaticText;

		bool _p_open = true;

		//IF we can display debug text
		if (ImGui::Begin(StaticText::DebugTextWindowID, &_p_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground)) {
			ImGui::PushFont(debugfont);

			//For each pending message
			for (auto& x : mMessages) {
				ImGui::TextColored({ x.mColor.r, x.mColor.g, x.mColor.b, x.mColor.a }, x.mText.c_str());
				x.mDurations.second += 0.016f;

				//If a message lifetime has expired, remove it
				if (x.mDurations.second >= x.mDurations.first) {
					if (mMessages.size() > 1)
						mMessages.remove(&x);
					else
						mMessages.clear();
				}
			}
			ImGui::PopFont();

			ImGui::End();
		}
	}

	// ------------------------------------------------------------------------
	/*! Get Buffer
	*
	*   Gets the Buffer the Editor is rendering the scene onto
	*/ // ---------------------------------------------------------------------
	GLuint ImGuiManager::GetBuffer() noexcept {
		return IsViewport() ? reinterpret_cast<Viewport*>(mStates[0]->GetWindow(6))->GetBuffer() : NULL;
	}

	// ------------------------------------------------------------------------
	/*! Get Viewport Dimensions
	*
	*   Get the size of the rendering viewport
	*/ // ---------------------------------------------------------------------
	Engine::Math::Vector2D ImGuiManager::GetViewportDimensions() noexcept {
		return reinterpret_cast<Viewport*>(mStates[0]->GetWindow(6))->GetViewportDimensions();
	}

	// ------------------------------------------------------------------------
	/*! IsViewport
	*
	*   Gets if we are rendering onto viewport or onto window
	*/ // ---------------------------------------------------------------------
	bool ImGuiManager::IsViewport() noexcept {
		return reinterpret_cast<Viewport*>(mStates[0]->GetWindow(6))->IsViewport();
	}
}

Editor::ImGuiManager& get_gui() {
	return Editor::ImGuiManager::Instance();
}
#endif