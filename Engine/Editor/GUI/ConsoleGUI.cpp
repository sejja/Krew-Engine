//
//  ConsoleGUI.cpp
//  Good Neighbours
//
//  Created by Diego Revilla 17/09/21.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Creates a Console Window
	*/ // ---------------------------------------------------------------------
	ConsoleWindow::ConsoleWindow() noexcept {
		memset(mInputBuf, NULL, sizeof(mInputBuf));
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys the Editor Console
	*/ // --------------------------------------------------------------------
	ConsoleWindow::~ConsoleWindow() noexcept {
		clearLog();
	}

	// ------------------------------------------------------------------------
	/*! Clear Log
	*
	*   Clears the Log of the console
	*/ // --------------------------------------------------------------------
	void ConsoleWindow::clearLog() noexcept {
		mItems.clear();
	}

	// ------------------------------------------------------------------------
	/*! Text Edit Callback Stub
	*
	*   ImGui Dependency
	*/ // --------------------------------------------------------------------
	constexpr int ConsoleWindow::textEditCallbackStub(ImGuiInputTextCallbackData*) noexcept {
		return 0;
	}

	// ------------------------------------------------------------------------
	/*! Add Log
	*
	*   Adds an Item to the console
	*/ // --------------------------------------------------------------------
	void ConsoleWindow::AddLog(const char* str) noexcept {
		mItems.emplace_back(strcpy(Engine::Allocator<char>::allocate(strlen(str) + 1), str));
		ImGui::SetScrollHereY(1.f);
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Displays the Console on the Guy
	*/ // --------------------------------------------------------------------
	void ConsoleWindow::Present() {
		const float _ftheight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		bool _reclaimfocus = false;

		auto& _console = *GConsole;

		//if we haven't created the console window
		if (ImGui::Begin("Console")) {
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -_ftheight), false,
				ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			//draw every single item stored
			for (auto& x : mItems) {
				bool _pop_color = false;
				//if it's an error
				if (strstr(x.get(), "ERROR")) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
					_pop_color = true;
				}
				else if (strstr(x.get(), "WARNING")) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
					_pop_color = true;
				}

				ImGui::TextUnformatted(x.get());

				//if we need to color the text
				if (_pop_color)
					ImGui::PopStyleColor();
			}

			if (mScrollToBottom)
				ImGui::SetScrollHereY(1.f);

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();
			mScrollToBottom = false;

			//If we have inputed a command
			if (ImGui::InputText("Input Command", mInputBuf, IM_ARRAYSIZE(mInputBuf),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory, &textEditCallbackStub, (void*)this)) {
				//If the command is valid
				if (mInputBuf) {
					AddLog(mInputBuf);

					//if the command is indeed clear
					if (!strcmp(mInputBuf, "clear"))
						clearLog();
					else {
						bool found = false;

						//Loop through all the commands
						for (Engine::Console::Command* x : _console.mCommands) {
							std::string cmd_(mInputBuf);

							//If we find a match
							if (!cmd_.rfind(x->command, 0)) {
								size_t pos = cmd_.find(x->command);

								//If we can extract the arguments
								if (pos != std::string::npos)
									cmd_.erase(pos, strlen(x->command));

								x->Process(cmd_.c_str());
								found = true;
								break;
							}
						}

						//If we could not find the command
						if (!found)
							AddLog("ERROR: Command not found");
					}
				}

				strcpy_s(mInputBuf, 1, "");
				_reclaimfocus = true;
			}

			//If we do have backlogs to print into the console
			if (!_console.mOutputBackLogStrings.empty()) {
				do {
					AddLog(_console.mOutputBackLogStrings.back().c_str());
					_console.mOutputBackLogStrings.pop_back();
					mScrollToBottom = true;

					//do it while we still have strings to show
				} while (!_console.mOutputBackLogStrings.empty());
			}

			ImGui::SetItemDefaultFocus();

			//If we should reclaim the focus of the HUD
			if (_reclaimfocus)
				ImGui::SetKeyboardFocusHere(-1);

			ImGui::End();
		}
	}
}

#endif