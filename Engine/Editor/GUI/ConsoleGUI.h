//
//  ConsoleGUI.h
//  Good Neighbours
//
//  Created by Diego Revilla 17/09/21.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifdef _EDITOR_
#ifndef _CONSOLE_GUI__H_
#define _CONSOLE_GUI__H_

namespace Editor {
	class ConsoleWindow : public IWindow {
	public:
#pragma region //Constructor & Destructor
		ConsoleWindow() noexcept;
		~ConsoleWindow() noexcept;
#pragma endregion
#pragma region //Functions
		void AddLog(const char* str) noexcept;
		void Present() override;
#pragma endregion
#pragma region //Members
	private:
		char mInputBuf[256];
		Engine::Array<Engine::UniquePointer<char>> mItems;
		constexpr static int textEditCallbackStub(ImGuiInputTextCallbackData* data) noexcept;
		void clearLog() noexcept;
		bool mScrollToBottom = true;
#pragma endregion
	};
}

#endif
#endif