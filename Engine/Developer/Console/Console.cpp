//
//	Console.cpp
//	Good Neighbours
//
//	Created by Diego Revilla 17/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Add Command
	*
	*   Adds a Command to the console
	*/ //--------------------------------------------------------------------
	void Console::AddCommand(Command* cmd) {
		mCommands.push_back(cmd);
	}

	Console& Console::Instance() noexcept {
		static Console cons;

		return cons;
	}

	// ------------------------------------------------------------------------
	/*! Log
	*
	*   Logs something onto screen
	*/ //--------------------------------------------------------------------
	void Console::Log(const eMessageType level, const char* fmt, ...) {
		va_list _ap;
		std::string _temp;

		va_start(_ap, fmt);

		switch (level) {
		case eMessageType::e_Error:
			_temp += "ERROR: ";
			break;

		case eMessageType::e_Warning:
			_temp += "WARNING: ";
			[[fallthrough]];
		default:
			break;
		}

		//For every character, determine what information to print
		for (char* p = const_cast<char*>(fmt); *p; p++) {
			//If it's a regular character
			if (*p != '%') {
				_temp.push_back(*p);

				continue;
			}

			//else, branch by data type
			switch (*++p) {
				//Integer
			case 'i':
				_temp += std::to_string(va_arg(_ap, int));

				break;

				//Float
			case 'f':
				_temp += std::to_string(va_arg(_ap, float));

				break;

				//String
			case 's':
				_temp += va_arg(_ap, char*);

				break;

				//Boolean
			case 'b':
				_temp += va_arg(_ap, bool) ? "true" : "false";

				break;

				//No case, just return the regular character
			default:
				_temp.push_back(*p);
			}
		}

		va_end(_ap);
		mOutputBackLogStrings.push_back(_temp);
	}
}