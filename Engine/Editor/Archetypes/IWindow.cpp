//
//  IWindow.h
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
	*   Constructs a Window Interface
	*/ // ---------------------------------------------------------------------
	IWindow::IWindow() noexcept :
		mActive(true), mOwnerState(nullptr) { }

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   EMPTY FUNCTION
	*/ // ---------------------------------------------------------------------
	IWindow::~IWindow() noexcept {}
}
#endif