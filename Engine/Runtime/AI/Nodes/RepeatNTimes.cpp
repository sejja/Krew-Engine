//
//	RepeatNTimes.cpp
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constucts a Repeat Node with the number of repetitions
	*/ // ---------------------------------------------------------------------
	RepeatNTimes::RepeatNTimes(const unsigned n) noexcept
		: mNumber(n), mCounter(0) {}

	// ------------------------------------------------------------------------
	/*! Update
	*
	*   Updates the node to repeat n ammount of times
	*/ // ---------------------------------------------------------------------
	Behavior::Status RepeatNTimes::Update() {
		Status stat = Status::RUNNING;

		while (mCounter++ != mNumber)
		{
			mChild->Tick();
			stat = mChild->GetStatus();
		}
		return stat;
	}

	// ------------------------------------------------------------------------
	/*! On Terminate
	*
	*   Called on this node last frame
	*/ // ---------------------------------------------------------------------
	void RepeatNTimes::OnTerminate() noexcept {
		mCounter = 0;
	}
}