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
	*   Constructs a Composite
	*/ // ---------------------------------------------------------------------
	Composite::Composite() noexcept {
		mStatus = Status::INVALID;
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees all children
	*/ // ---------------------------------------------------------------------
	Composite::~Composite() {
		for (auto& child : mChildren)
			mOwner->FreeBehavior(child);
	}

	// ------------------------------------------------------------------------
	/*! Add Child
	*
	*   Adds ownership of a state
	*/ // ---------------------------------------------------------------------
	void Composite::AddChild(Behavior* const state) {
		mChildren.push_back(state);
		state->SetOwner(mOwner);
	}

	// ------------------------------------------------------------------------
	/*! Remove Child
	*
	*   Removes Ownership of a child
	*/ // ---------------------------------------------------------------------
	void Composite::RemoveChild(Behavior* const state) {
		mChildren.remove(state);
	}

	Array<Behavior*> Composite::GetChildren() const
	{
		return mChildren;
	}
}