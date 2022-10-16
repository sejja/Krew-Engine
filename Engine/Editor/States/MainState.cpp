//
//	MainState.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	static std::atomic<unsigned char> timeafterload = 0;

	void AutoSave() {
		using namespace std::literals;

		for (;;) {
			std::this_thread::sleep_for(60s);
			if (!GEditor->IsPlaying()) {

				CHAR my_documents[MAX_PATH];
				HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

				if (result == S_OK) {
					nlohmann::json _outjson;
					GWorld->GetScene()->ToJson(_outjson);
					std::string path = std::string(my_documents) + "\\Backup.level";
					std::ofstream _output(path);
					_output << _outjson;
					_output.close();
					timeafterload = 0xFF;
				}
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs the Main State, with windows and default values
	*/ //----------------------------------------------------------------------
	MainState::MainState() noexcept
		: mFirstResourcesPending(true), mTotalMissingResources(0),
		mCurrentLevel(__Texts::Editor::MainState::StaticText::nulltext)
		, mAutoSave(AutoSave) {
		using __Texts::Editor::MainState::StaticText;
		const auto f_addinactivewindow = [this](IWindow* const win) {
			win->SetActive(false);
			AddWindow(win);
		};

		SetName(StaticText::Title);
		f_addinactivewindow(Engine::Allocator<ConsoleWindow>::New());
		f_addinactivewindow(Engine::Allocator<ResourceWindow>::New());
		f_addinactivewindow(Engine::Allocator<MemoryTrackerWindow>::New());
		AddWindow(Engine::Allocator<PropertiesWindow>::New());
		AddWindow(Engine::Allocator<SceneWindow>::New());
		AddWindow(Engine::Allocator<ExplorerWindow>::New());
		AddWindow(Engine::Allocator<Viewport>::New());
		f_addinactivewindow(Engine::Allocator<FPSProfiler>::New());
		AddWindow(Engine::Allocator<PaletteWindow>::New());

		if (std::filesystem::exists(TEXT("_lock"))) {

			AddWindow(Engine::Allocator<LevelRetrievalModal>::New());
		}

		std::ofstream _output(TEXT("_lock"));
		_output.close();
	}

	MainState::~MainState() {
	}

	// ------------------------------------------------------------------------
	/*! Buffering Bar
	*
	*   Draws a Buffering Bar Widget
	*/ //----------------------------------------------------------------------
	void MainState::bufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) const {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg; size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ImGui::ItemSize(bb, style.FramePadding.y);

		if (!ImGui::ItemAdd(bb, id))
			return;

		const float circleStart = size.x * 0.7f;
		const float circleEnd = size.x;
		const float circleWidth = circleEnd - circleStart;
		const float t = static_cast<float>(g.Time);
		const float r = size.y / 2;
		const float speed = 1.5f;
		const float a = speed * 0;
		const float b = speed * 0.333f;
		const float c = speed * 0.666f;
		const float widgetplusradious = circleWidth + r;

		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - widgetplusradious * (t + a - speed * static_cast<int>((t + a) / speed)) / speed, bb.Min.y + r),
			r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - widgetplusradious * (t + b - speed * static_cast<int>((t + b) / speed)) / speed, bb.Min.y + r),
			r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - widgetplusradious * (t + c - speed * static_cast<int>((t + c) / speed)) / speed, bb.Min.y + r),
			r, bg_col);
	}

	// ------------------------------------------------------------------------
	/*! Spinner
	*
	*   Draws an Spinner on ImGui
	*/ //----------------------------------------------------------------------
	void MainState::spinner(const char* label, float radius, int thickness, const ImU32& color) const {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 pos = window->DC.CursorPos;
		const ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);
		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));

		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return;

		// Render
		window->DrawList->PathClear();

		constexpr int num_segments = 30;
		constexpr float two_pi = PI * 2.f;

		const float a_min = two_pi * (static_cast<float>(fabs(Engine::Math::Sin(static_cast<float>(g.Time) * 1.8f))* (num_segments - 5u))) / static_cast<float>(num_segments);
		const float a_max = two_pi * (static_cast<float>(num_segments) - 3) / static_cast<float>(num_segments);

		const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

		for (int i = 0; i < num_segments; i++) {
			const float rad = a_min + (static_cast<float>(i) / static_cast<float>(num_segments))* (a_max - a_min) + static_cast<float>(g.Time) * 8;
			window->DrawList->PathLineTo(ImVec2(centre.x + Engine::Math::Cos(rad) * radius,
				centre.y + Engine::Math::Sin(rad) * radius));
		}

		window->DrawList->PathStroke(color, ImDrawFlags_None, static_cast<float>(thickness));
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the header
	*/ //--------------------------------------------------------------------
	void MainState::Present() noexcept {
		using __Texts::Editor::MainState::StaticText;

		//If we can display the top bar
		if (ImGui::BeginMainMenuBar()) {
			ImGui::MenuItem(StaticText::BarTitle);

			//IF we are in the File Menu
			if (ImGui::BeginMenu(StaticText::File)) {
				//If we want to create a new scene
				if (ImGui::MenuItem(StaticText::NewScene)) {
					GWorld->GetScene()->Clear();
					GEditor->GetSelectedObject().clear();
				}

				if (!mCurrentLevel.empty()) {
					if (ImGui::MenuItem(StaticText::Save, StaticText::SaveShortcut)) {
						nlohmann::json _outjson;
						GWorld->GetScene()->ToJson(_outjson);
						std::ofstream _output(mCurrentLevel);
						_output << std::setw(4);
						_output << _outjson;
						_output.close();
					}
				}
				else {
					ImGui::MenuItem(StaticText::Save, StaticText::SaveShortcut, nullptr, false);
				}

				if (ImGui::MenuItem(StaticText::SaveAs))
					AddWindow(Engine::Allocator<ImporterWindowModal>::New());

				ImGui::EndMenu();
			}

			if (!mCurrentLevel.empty() && GInput->IsKeyDown(VK_CONTROL) && GInput->IsKeyPressed('S')) {
				nlohmann::json _outjson;
				GWorld->GetScene()->ToJson(_outjson);
				std::ofstream _output(mCurrentLevel);
				_output << _outjson;
				_output.close();
			}

			//Create an Options Section
			if (ImGui::BeginMenu(StaticText::View)) {
				const auto _feditwindowvisibility = [this](const char* str, size_t i) {
					Editor::IWindow* const _window = GetWindow(i);
					const bool _b = _window->IsActive();

					if (ImGui::MenuItem(str, nullptr, _b)) _window->SetActive(!_b);
				};

				_feditwindowvisibility(StaticText::ShowConsole, 0);
				_feditwindowvisibility(StaticText::ShowProjectExplorer, 5);
				ImGui::Separator();
				_feditwindowvisibility(StaticText::ShowFPSProfiler, 7);
				_feditwindowvisibility(StaticText::ShowStreamedResources, 1);
				_feditwindowvisibility(StaticText::ShowMemoryTracker, 2);

				ImGui::EndMenu();
			}

			//Create an Options Section
			if (ImGui::BeginMenu(StaticText::Source)) {
				auto& loader = *GBinaryLoader;

				if (ImGui::MenuItem(StaticText::HotReload)) loader.ReloadSource();
				if (ImGui::MenuItem(StaticText::HotReloadAndBuild)) loader.CompileSource();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//For every window
		for (size_t i = 0, count = GetWindowCount(); i < count; i++) {
			Editor::IWindow* const x = GetWindow(i);

			//If the window is active
			if (x->IsActive())
				x->Present();
		}

		auto selection = GEditor->GetSelectedObject();

		if (GInput->IsKeyDown(VK_CONTROL) && GInput->IsKeyPressed('C')) {

			nlohmann::json cliptext;

			for (auto& x : selection) {
				cliptext.emplace_back();
				auto& currjson = cliptext.back();

				x->ToJson(currjson);
			}

			std::string source = cliptext.dump();
			if (OpenClipboard(GWindow->GetHandle())) {
				HGLOBAL clipbuffer;
				char* buffer;
				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.size() + 1);
				buffer = (char*)GlobalLock(clipbuffer);
				strcpy(buffer, LPCSTR(source.c_str()));
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT, clipbuffer);
				CloseClipboard();
			}
		}

		if (GInput->IsKeyDown(VK_LCONTROL) && GInput->IsKeyPressed('V')) {
			char* buffer;
			if (OpenClipboard(NULL)) {
				GEditor->GetSelectedObject().clear();
				buffer = (char*)GetClipboardData(CF_TEXT);
				std::string text(buffer);
				auto newjson = nlohmann::json::parse(text);
				for (auto& x : newjson) {
					auto newgo = GWorld->GetScene()->SpawnObject();
					newgo->FromJson(x);
					GEditor->SetSelectedObject(newgo);
				}
			}

			CloseClipboard();
		}

		auto& content = *GContent;
		if (timeafterload) {
			ImGuiToast toast(ImGuiToastType_Info, 3000);
			toast.set_title(TEXT("Backup"));
			toast.set_content(TEXT("Level Auto-Saved"));
			ImGui::InsertNotification(toast);
			timeafterload = 0;
		}

		if (content.PendingResources()) {
			if (ImGui::Begin(StaticText::ProgressBar, 0, ImGuiWindowFlags_NoTitleBar)) {
				if (mFirstResourcesPending) mTotalMissingResources = content.PendingResources();
				mFirstResourcesPending = false;

				const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);

				spinner(StaticText::SpinnerID, 15, 6, col);
				bufferingBar(StaticText::BufferBarID, 0.7f, ImVec2(400, 6), ImGui::GetColorU32(ImGuiCol_Button), col);
				ImGui::End();
			}
		}
		else {
			if (mTotalMissingResources) {
				ImGui::InsertNotification({ ImGuiToastType_Success, 3000, TEXT("All content streamed to memory succesfully")});
			}

			mFirstResourcesPending = true;
			mTotalMissingResources = 0;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
		ImGui::RenderNotifications(); // <-- Here we render all notifications
		ImGui::PopStyleVar(1); // Don't forget to Pop()
	}
}

#endif