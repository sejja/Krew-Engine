//
//	CodeTracker.h
//	Good Neighbours
//
//	Created by Diego Revilla on 22/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef __cpp_consteval
#ifndef _CODE_TRACKER__H_
#define _CODE_TRACKER__H_

namespace Engine {
	struct CodeTracker {
		static consteval CodeTracker Current(const char* file = __builtin_FILE(), const char* function = __builtin_FUNCTION()) noexcept {
			CodeTracker _temp;
			_temp.mFile = file;
			_temp.mFunction = function;
			return _temp;
		}

		constexpr CodeTracker() noexcept = default;

		constexpr const char* File() const noexcept {
			return mFile;
		}
		constexpr const char* Function() const noexcept {
			return mFunction;
		}

	private:
		const char* mFile = "";
		const char* mFunction = "";
	};
}

#endif
#endif