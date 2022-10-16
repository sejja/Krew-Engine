//
//	Composite.cpp
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs a Decorator
	*/ // ---------------------------------------------------------------------
	Decorator::Decorator() noexcept :
		mChild(nullptr) {
		// set the types
		mStatus = Status::INVALID;
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees the owned child
	*/ // ---------------------------------------------------------------------
	Decorator::~Decorator() {
		// free the child
		mOwner->FreeBehavior(mChild);
	}

	// ------------------------------------------------------------------------
	/*! Add Child
	*
	*   Adds ownership over a child
	*/ // ---------------------------------------------------------------------
	void Decorator::AddChild(Behavior* const state) noexcept {
		//add new child
		mChild = state;
		state->SetOwner(mOwner);
	}
}