//
//	Sequences.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _SEQUENCER__H_
#define _SEQUENCER__H_

namespace Engine  {
	struct ENGINE_API Sequencer : public Composite {
#pragma region // Constructors & Destructors
		Sequencer() noexcept;
#pragma endregion

#pragma region // Functions
	protected:
		virtual void OnInitialize() noexcept final override;
		virtual Status Update() final override;
#pragma endregion

#pragma region // Members
		Array<Behavior*>::iterator mChild;
#pragma endregion
	};
}

#endif // !SEQUENCER_H_