//
//  IWindow.h
//  Good Neighbours
//
//  Created by Diego Revilla 17/09/21.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifdef _EDITOR_
#ifndef _I_WINDOW__H_
#define _I_WINDOW__H_

namespace Editor {
	struct IEditorState;

	struct ENGINE_API IWindow {
	public:
		IWindow() noexcept;
		virtual ~IWindow() noexcept;
		virtual void Present() = 0;
		DONTDISCARD bool inline IsActive() const noexcept;
		void inline SetActive(const bool b) noexcept;
		void inline SetOwnerState(IEditorState* const state) noexcept;
		DONTDISCARD IEditorState inline* GetOwner() const noexcept;
	private:
		bool mActive = false;
		IEditorState* mOwnerState;
	};

	// ------------------------------------------------------------------------
	/* Is Active
	*
	*   Returns wether a Window is active or not
	*/ // ---------------------------------------------------------------------
	bool inline IWindow::IsActive() const noexcept {
		return mActive;
	}

	// ------------------------------------------------------------------------
	/* Set Active
	*
	*   Sets a Window as Active or not
	*/ // ---------------------------------------------------------------------
	void inline IWindow::SetActive(const bool b) noexcept {
		mActive = b;
	}

	// ------------------------------------------------------------------------
	/* Set Owner State
	*
	*   Sets the Owner of this window
	*/ // ---------------------------------------------------------------------
	void inline IWindow::SetOwnerState(IEditorState* const state) noexcept {
		mOwnerState = state;
	}

	// ------------------------------------------------------------------------
	/* Get Owner
	*
	*   Gets the State Owner of this Window
	*/ // ---------------------------------------------------------------------
	IEditorState inline* IWindow::GetOwner() const noexcept {
		return mOwnerState;
	}
}

#endif
#endif