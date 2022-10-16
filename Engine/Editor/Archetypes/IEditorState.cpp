//
//	IEditorState.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 21/11/20
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents all the windows of one sgtate
	*/ // ---------------------------------------------------------------------
	void IEditorState::Present() {
		Engine::FForEach(mWindows.begin(), mWindows.end(), [](Engine::UniquePointer<IWindow>& x) {if (x->IsActive())x->Present(); });
	}

	// ------------------------------------------------------------------------
	/*! Add Window
	*
	*   Adds a window to the state
	*/ // --------------------------------------------------------------------
	void IEditorState::AddWindow(IWindow* win) noexcept {
		win->SetOwnerState(this);
		mWindows.emplace_back(win);
	}

	// ------------------------------------------------------------------------
	/*! Remove Window
	*
	*   Removes a window from the State
	*/ // ---------------------------------------------------------------------
	void IEditorState::RemoveWindow(IWindow* win) noexcept {
		for (auto it = mWindows.begin(), end = mWindows.end(); it != end; it++)
			if (*it == win) {
				mWindows.remove(it);
				break;
			}
	}
}
#endif