//
//	Component.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 11/02/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Default Constructors
	*
	*   Sets the Owner to nullptr
	*/ // ---------------------------------------------------------------------
	Component::Component() noexcept :
		mOwner(nullptr) {}
}