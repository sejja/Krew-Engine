//
//	Console.h
//	Good Neighbours
//
//	Created by Diego Revilla 17/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _CONSOLE__H_
#define _CONSOLE__H_

namespace Editor {
	class ConsoleWindow;
}

namespace Engine {
	class ENGINE_API Console {
#pragma region //Declarations
		friend Editor::ConsoleWindow;
	public:
		Console& operator=(const Console&) = delete;

		enum class eMessageType : unsigned char {
			e_Message,
			e_Error,
			e_Warning
		};

		struct Command {
		public:
			virtual void Process(const char* args) = 0;
			const char* command = nullptr;
		};
#pragma endregion
#pragma region //Functions
		static Console& Instance() noexcept;
		void AddCommand(Command* cmd);
		void Log(const eMessageType level, const char* format, ...);
#pragma endregion
#pragma region // Members
	private:
		Array<std::string> mOutputBackLogStrings;
		Array<Command*> mCommands;
#pragma endregion
	};
}

#define GConsole (&Engine::Console::Instance())

#endif _CONSOLE__H_