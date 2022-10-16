//
//	MainState.h
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _HEADER__H_
#define _HEADER__H_

namespace Editor {
	struct MainState : public IEditorState {
#pragma region // Constructors & Destrucors
		MainState() noexcept;
		~MainState();
#pragma endregion

#pragma region // Functions
		void Present() noexcept override;
	private:
		void spinner(const char* label, float radius, int thickness, const ImU32& color) const;
		void bufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) const;
#pragma endregion

#pragma region // Members
		bool mFirstResourcesPending = true;
		unsigned mTotalMissingResources = 0;
	public:
		std::string mCurrentLevel;
		std::thread mAutoSave;
#pragma endregion
	};
}

#endif
#endif