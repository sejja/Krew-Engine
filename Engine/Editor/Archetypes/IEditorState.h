//
//	IEditorState.h
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _I_EDITOR_STATE__H_
#define _I_EDITOR_STATE__H_

namespace Editor {
	struct IEditorState {
	public:
#pragma region // Functions
		virtual void Present();
		void AddWindow(IWindow* const win) noexcept;
		void RemoveWindow(IWindow* const win) noexcept;
		void inline SetName(const char* str) noexcept;
		DONTDISCARD IWindow inline* GetWindow(const size_t idx) noexcept;
		DONTDISCARD size_t inline GetWindowCount() const noexcept;
		DONTDISCARD const char inline* GetName() const noexcept;
#pragma endregion

#pragma region // Members
	private:
		std::string mName;
		Engine::Array<Engine::UniquePointer<IWindow>> mWindows;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Open Level
	*
	*   Gets a window from the state given the index
	*/ // ---------------------------------------------------------------------
	DONTDISCARD IWindow inline* IEditorState::GetWindow(const size_t idx) noexcept {
		return mWindows[idx].get();
	}

	// ------------------------------------------------------------------------
	/*! Get Window Count
	*
	*   Returns the Total Window Count
	*/ // ---------------------------------------------------------------------
	DONTDISCARD size_t inline IEditorState::GetWindowCount() const noexcept {
		return mWindows.size();
	}

	// ------------------------------------------------------------------------
	/*! Get Name
	*
	*   Returns the State Name
	*/ // ---------------------------------------------------------------------
	DONTDISCARD const char inline* IEditorState::GetName() const noexcept {
		return mName.c_str();
	}

	// ------------------------------------------------------------------------
	/*! Set Name
	*
	*   Sets the Name of the State
	*/ // ---------------------------------------------------------------------
	void inline IEditorState::SetName(const char* str) noexcept {
		mName = str;
	}
}

#endif
#endif