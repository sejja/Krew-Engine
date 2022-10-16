//
//	FrameController.h
//	Good Neighbours
//
//	Created by Diego Revilla 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _FRAME_CONTROLLER__H_
#define _FRAME_CONTROLLER__H_

namespace Engine {
	namespace Frames {
		class ENGINE_API FrameController {
		public:
			using precision_type = double;
			using size_type = size_t;
			FrameController() noexcept;

			void Update() noexcept;
			void Reset() noexcept;
			void SetMaxFrameRate(precision_type fps) noexcept;
			static FrameController& Instance();
			DONTDISCARD precision_type inline GetMaxFrameRate() const noexcept;
			DONTDISCARD precision_type inline GetFrameRate() const noexcept;
			DONTDISCARD precision_type inline GetFrameTime() const noexcept;
			DONTDISCARD precision_type inline GetMenuFrameTime() const noexcept;
			DONTDISCARD size_type inline GetFrameCounter() const noexcept;
			void inline LockFrameRate(bool enabled) noexcept;
			DONTDISCARD bool inline FrameRateLocked() const noexcept;
			DONTDISCARD precision_type inline GetLastFrameTime() const noexcept;
			DONTDISCARD precision_type inline GetGlobalDilation() noexcept;
			void inline SetGlobalDilation(float time) noexcept;
			double inline& GraphicsTime() noexcept;
			double inline& PhysicsTime() noexcept;
			double inline& LogicTime() noexcept;
		private:
			bool mbFrameRateLocked = true;
			precision_type mFrameRateMax = 0, mFrameRate = 0,
				mFrameTime = 0, mFrameTimeMin = 0.f,
				mDilationTime = 1.f, mPhysicsTime = 0, mGraphicsTime = 0, mLogicTime = 0;
			std::chrono::steady_clock::time_point mFrameTimeStart, mFrameTimeEnd;
			size_type mFrameCounter = 0;
		};

		// ------------------------------------------------------------------------
		/*! Create Win32 Window Class
		*
		*   Registers a new class on the OS
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::precision_type inline FrameController::GetMaxFrameRate() const noexcept {
			return mFrameRateMax;
		}

		// ------------------------------------------------------------------------
		/*! Get Frame Rate
		*
		*   Gets the Frame Rate
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::precision_type inline FrameController::GetFrameRate() const noexcept {
			return mFrameRate;
		}

		// ------------------------------------------------------------------------
		/*! Get Frame Time
		*
		*   Gets the Frame time
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::precision_type inline FrameController::GetFrameTime() const noexcept {
			return mFrameTime * mDilationTime;
		}

		// ------------------------------------------------------------------------
		/*! Get Frame Time
		*
		*   Gets the Frame time
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::precision_type inline FrameController::GetMenuFrameTime() const noexcept {
			return mFrameTime;
		}

		// ------------------------------------------------------------------------
		/*! Get Frame Counter
		*
		*   Gets the Frame Count
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::size_type inline FrameController::GetFrameCounter() const noexcept {
			return mFrameCounter;
		}

		// ------------------------------------------------------------------------
		/*! Lock Frame Rate
		*
		*   Locks the Frame Rate
		*/ // ---------------------------------------------------------------------
		void inline FrameController::LockFrameRate(bool enabled) noexcept {
			mbFrameRateLocked = enabled;
		}

		// ------------------------------------------------------------------------
		/*! Frame Rate Locked
		*
		*   Gets if the Frame Rate Is Locked
		*/ // ---------------------------------------------------------------------
		DONTDISCARD bool inline FrameController::FrameRateLocked() const noexcept {
			return mbFrameRateLocked;
		}

		// ------------------------------------------------------------------------
		/*! Get Last Frame Time
		*
		*   Gets the time spent the last frame
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::precision_type inline FrameController::GetLastFrameTime() const noexcept {
			return mFrameTime * mDilationTime;
		}

		// ------------------------------------------------------------------------
		/*! Get Global Dilation
		*
		*   Gets the Global time Dilation
		*/ // ---------------------------------------------------------------------
		DONTDISCARD FrameController::precision_type inline FrameController::GetGlobalDilation() noexcept {
			return mDilationTime;
		}

		// ------------------------------------------------------------------------
		/*! Set Global Dilation
		*
		*   Sets the Global time Dilation
		*/ // ---------------------------------------------------------------------
		void inline FrameController::SetGlobalDilation(float time) noexcept {
			mDilationTime = time;
		}

		// ------------------------------------------------------------------------
		/*! Graphics Time
		*
		*   Retrieves the Graphics Time Variable
		*/ // ---------------------------------------------------------------------
		double inline& FrameController::GraphicsTime() noexcept {
			return mGraphicsTime;
		}

		// ------------------------------------------------------------------------
		/*! Graphics Time
		*
		*   Retrieves the Physics Time Variable
		*/ // ---------------------------------------------------------------------
		double inline& FrameController::PhysicsTime() noexcept {
			return mPhysicsTime;
		}

		// ------------------------------------------------------------------------
		/*! Graphics Time
		*
		*   Retrieves the Logic Time Variable
		*/ // ---------------------------------------------------------------------
		double inline& FrameController::LogicTime() noexcept {
			return mLogicTime;
		}
	}
}

#define GFrames (&Engine::Frames::FrameController::Instance())

#endif