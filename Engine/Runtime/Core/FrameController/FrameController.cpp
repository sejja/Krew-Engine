//
//	FrameController.cpp
//	Good Neighbours
//
//	Created by Diego Revilla 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Frames {
		// ------------------------------------------------------------------------
		/*! Defailt Constructor
		*
		*   Initializes the Variables
		*/ // ---------------------------------------------------------------------
		FrameController::FrameController() noexcept
			: mFrameCounter(0), mFrameRateMax(60), mFrameRate(mFrameRateMax), mFrameTime(mFrameRate),
			mFrameTimeMin(1.f / mFrameRateMax) {}

		// ------------------------------------------------------------------------
		/*! Instance
		*
		*   Returns a local instance of the class
		*/ // ---------------------------------------------------------------------
		FrameController& FrameController::Instance() {
			static FrameController t;
			return t;
		}

		// ------------------------------------------------------------------------
		/*! Reset
		*
		*   Resets the Frame Counter
		*/ // --------------------------------------------------------------------
		void FrameController::Reset() noexcept {
			mFrameCounter = 0;
			mFrameRate = mFrameRateMax;
			mFrameTime = 1.f / mFrameRate;
			mFrameTimeMin = 1.f / mFrameRateMax;
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Frame
		*/ // --------------------------------------------------------------------
		void FrameController::Update() noexcept {
			const auto fgetframtime = [this]() {
				mFrameTimeEnd = std::chrono::steady_clock::now();
				mFrameTime = (mFrameTimeEnd - mFrameTimeStart).count() / 1000000000.0; // Nanoseconds
			};

			fgetframtime();

			//Sleep if we have spare time
			if (mbFrameRateLocked && mFrameTimeMin > mFrameTime) {
				std::this_thread::sleep_for(mFrameTimeEnd - mFrameTimeStart); // Miliseconds
				fgetframtime();
			}

			mFrameRate = 1.f / mFrameTime;
			mFrameCounter++;
			mFrameTimeStart = std::chrono::steady_clock::now();
		}

		// ------------------------------------------------------------------------
		/*! Set the Maximun Frame Rate
		*
		*   Sets a Maximun Frame Rate
		*/ // --------------------------------------------------------------------
		void FrameController::SetMaxFrameRate(precision_type fps) noexcept {
			mFrameRateMax = fps;
			mFrameTimeMin = 1.f / mFrameRateMax;
		}
	}
}