//
//	RepeatNTimes.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _REPEAT_N_TIMES__H_
#define _REPEAT_N_TIMES__H_

namespace Engine {
	struct ENGINE_API RepeatNTimes : public Decorator {
#pragma region // Constructors & Destructors
		explicit RepeatNTimes(const unsigned n) noexcept;
#pragma endregion

#pragma region // Functions
		virtual Status Update() final override;
		virtual void OnTerminate() noexcept final override;
#pragma endregion

#pragma region // Members
	private:
		unsigned mNumber;
		unsigned mCounter;
#pragma endregion
	};
}

#endif // !REPEATNTIMES_H_



