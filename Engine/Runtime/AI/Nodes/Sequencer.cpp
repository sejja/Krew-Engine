//
//	Sequences.cpp
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
	*   Constructs a Sequencer & Composite
	*/ // ---------------------------------------------------------------------
	Sequencer::Sequencer() noexcept
		: Composite(), mChild(nullptr) {}

	// ------------------------------------------------------------------------
	/*! On Initialize
	*
	*   Sets the Child to the begin of the children list
	*/ // ---------------------------------------------------------------------
	void Sequencer::OnInitialize() noexcept {
		mChild = mChildren.begin();
	}

	// ------------------------------------------------------------------------
	/*! Update
	*
	*   Updates a Sequencer, Traversing through it's children
	*/ // ---------------------------------------------------------------------
	Behavior::Status Sequencer::Update(){
		//iterates on the children until one returns a fail
		for(;;) {
			(*mChild)->Tick();
			const Status stat = (*mChild)->GetStatus();
			if (stat == Status::FAIL) return stat;
			if (stat == Status::RUNNING) return stat;
			if(stat == Status::SUCCESS) ++mChild;
			if (mChild == mChildren.end()) return Status::SUCCESS;
		}
		
		// if loop exits unexpectedly;
		return Status::INVALID;
	}
}