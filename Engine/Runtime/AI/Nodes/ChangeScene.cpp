//
//	ChangeScene.cpp
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include "Shared.h"

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Calls the Leaf Constructor
	*/ // ---------------------------------------------------------------------
	ChangeScene::ChangeScene() noexcept
		: Leaf() {}

	// ------------------------------------------------------------------------
	/*! On Initialize
	*
	*   Called whenever we initialize the Leaf
	*/ // ---------------------------------------------------------------------
	void ChangeScene::OnInitialize() noexcept {
		mStatus = Status::RUNNING;
	}

	// ------------------------------------------------------------------------
	/*! On Update
	*
	*   Called whenever we update this leaf (once per frame)
	*/ // ---------------------------------------------------------------------
	Behavior::Status ChangeScene::Update() noexcept {
		return Status::SUCCESS;
	}

	// ------------------------------------------------------------------------
	/*! On Terminate
	*
	*   Called whenever we change nodes
	*/ // ---------------------------------------------------------------------
	void ChangeScene::OnTerminate() noexcept {
		GWorld->ChangeLevel("dummy.json");
	}
}