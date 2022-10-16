//
//	Interpolation.cpp
//	Good Neighbours
//
//	Created by Adrian Montes on 22/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Math {
		namespace Interpolation {
#pragma region INTERPOLATION
			// ------------------------------------------------------------------------
			/*! EaseLinear
			*
			*   Given a value between [0,1], does EaseLinear (no effect)
			*/ // ---------------------------------------------------------------------
			float EaseLinear(float t) noexcept
			{
				return t;
			}
			// ------------------------------------------------------------------------
			/*! EaseInQuad
			*
			*   Given a value between [0,1], does EaseInQuad (Accelerates toward goal)
			*/ // ---------------------------------------------------------------------
			float EaseInQuad(float t) noexcept
			{
				return t * t;
			}
			// ------------------------------------------------------------------------
			/*! EaseOutQuad
			*
			*   Given a value between [0,1], does EaseInQuad (Deccelerates toward goal)
			*/ // ---------------------------------------------------------------------
			float EaseOutQuad(float t) noexcept
			{
				return t * (2 - t);
			}
			// ------------------------------------------------------------------------
			/*! EaseOutQuad
			*
			*   Given a value between [0,1], does EaseInOutQuad (First half accelerates,
				second half deccelerates toward goal)
			*/ // ---------------------------------------------------------------------
			float EaseInOutQuad(float t) noexcept
			{
				if (2 * t <= 1)
					return EaseInQuad((2.f * t)) * 0.5f;
				else
					return EaseOutQuad((2.f * t - 1)) * 0.5f + 0.5f;
			}
#pragma endregion

#pragma region CURVES
			// ------------------------------------------------------------------------
			/*! Curve
			*
			*   Default constructor
			*/ // ---------------------------------------------------------------------
			Curve::Curve() noexcept : mIsInterpolating(false), mLoop(false), mRegion(0)
			{
			}
			// ------------------------------------------------------------------------
			/*! ~Curve
			*
			*   Default destructor
			*/ // ---------------------------------------------------------------------
			Curve::~Curve() noexcept
			{
				//Delete all the curve regions
				for (auto it = mCurves.begin(); it != mCurves.end(); it++)
					delete* it;
				mCurves.clear();
			}
			// ------------------------------------------------------------------------
			/*! operator=
			*
			*   Copy constructor
			*/ // ---------------------------------------------------------------------
			Curve& Curve::operator=(const Curve& rhs)
			{
				mIsInterpolating = rhs.mIsInterpolating;
				mLoop = rhs.mLoop;
				mRegion = rhs.mRegion; 

				for (size_t i = 0; i < rhs.mCurves.size(); i++)
				{
					Vector3D p0 = rhs.mCurves[i]->mInit;
					Vector3D p1 = rhs.mCurves[i]->mEnd;
					Interpolation::Interpolators fun = rhs.mCurves[i]->mFunc;
					float *time = &rhs.mCurves[i]->mTimer;
					float speed = rhs.mCurves[i]->mSpeed;

					region_info* ri = new region_info(p0, p1, fun, time, speed);

					mCurves.push_back(ri);
				}

				return *this;
			}

			// ------------------------------------------------------------------------
			/*! AddRegion
			*
			*   Adds a new region to the curve.
			*/ // ---------------------------------------------------------------------
			void Curve::AddRegion(const Vector3D& start, const Vector3D& end, Interpolation::Interpolators type, float* t, float speed) noexcept
			{
				mCurves.push_back(new region_info(start, end, type, t, speed));
			}
			// ------------------------------------------------------------------------
			/*! RemoveRegion
			*
			*   Removes region from curve
			*/ // ---------------------------------------------------------------------
			void Curve::RemoveRegion(unsigned r) noexcept
			{
				//Sanity check
				if (r >= mCurves.size())
					return;

				//Get region before removing from curve
				auto it = mCurves[r];
				mCurves.erase(mCurves.begin() + r);

				//Delete memory
				delete it;
			}
			// ------------------------------------------------------------------------
			/*! SetLoop
			*
			*   Loop mode settor
			*/ // ---------------------------------------------------------------------
			void Curve::SetLoop(bool l) noexcept
			{
				mLoop = l;
			}
			// ------------------------------------------------------------------------
			/*! Play
			*
			*   Curve interpolation is activated
			*/ // ---------------------------------------------------------------------
			void Curve::Play() noexcept
			{
				mIsInterpolating = true;
			}
			// ------------------------------------------------------------------------
			/*! Pause
			*
			*   Curve interpolation is deactivated
			*/ // ---------------------------------------------------------------------
			void Curve::Pause() noexcept
			{
				mIsInterpolating = false;
			}
			// ------------------------------------------------------------------------
			/*! Update
			*
			*   Updates curve interpolation
			*/ // ---------------------------------------------------------------------
			void Curve::Update() noexcept
			{
				//If not interpolating, don't do anything
				if (!mIsInterpolating || mCurves.empty())
					return;

				//Make time variable pointer to make it easier and update every frame
				float& t = mCurves[mRegion]->mTimer;
				float s = mCurves[mRegion]->mSpeed;
				t += FRAMERATE / s;

				//Use stored function to interpolate timer
				switch (mCurves[mRegion]->mFunc)
				{
				case Interpolation::Interpolators::LINEAR:
					*mCurves[mRegion]->mUserTime = Interpolation::EaseLinear(t);
					break;
				case Interpolation::Interpolators::IN_QUAD:
					*mCurves[mRegion]->mUserTime = Interpolation::EaseInQuad(t);
					break;
				case Interpolation::Interpolators::OUT_QUAD:
					*mCurves[mRegion]->mUserTime = Interpolation::EaseOutQuad(t);
					break;
				case Interpolation::Interpolators::IN_OUT_QUAD:
					*mCurves[mRegion]->mUserTime = Interpolation::EaseInOutQuad(t);
					break;
				}

				//Clamp time between [0,1]
				//(Note: This can be change to add a new maximum time)
				t = Clamp(t, 0.f, 1.f);
				if (t == 1.0f)
				{
					//If max time reached, start new curve region
					mRegion++;
					if (mRegion == mCurves.size())
					{
						//If last region done, stop interpolating
						//Unless the curve is looping
						mRegion--;
						if (mLoop)
							ResetCurve();
						else
							mIsInterpolating = false;
					}
				}
			}
			// ------------------------------------------------------------------------
			/*! IsInterpolating
			*
			*   Gettor to know if curve is interpolating
			*/ // ---------------------------------------------------------------------
			bool Curve::IsInterpolating() const noexcept
			{
				return mIsInterpolating;
			}
			// ------------------------------------------------------------------------
			/*! IsLooping
			*
			*   Gettor to know if curve is looping
			*/ // ---------------------------------------------------------------------
			bool Curve::IsLooping() const noexcept
			{
				return mLoop;
			}
			// ------------------------------------------------------------------------
			/*! GetRegion
			*
			*   Gettor to know in which region is the curve
			*/ // ---------------------------------------------------------------------
			unsigned Curve::GetRegion() const noexcept
			{
				return mRegion;
			}
			// ------------------------------------------------------------------------
			/*! ResetCurve
			*
			*   Sets all the curve regions to their initial time
			*/ // ---------------------------------------------------------------------
			void Curve::ResetCurve() noexcept
			{
				for (auto it : mCurves)
					*(it->mUserTime) = it->mTimer = it->mInitTime = 0.0f;
			}
			// ------------------------------------------------------------------------
			/*! CurveIsEmpty
			*
			*   True if no regions, false otherwise
			*/ // ---------------------------------------------------------------------
			bool Curve::CurveIsEmpty() const noexcept
			{
				return mCurves.empty();
			}

			// ------------------------------------------------------------------------
			/*! region_info
			*
			*   Constructor of curve region info
			*/ // ---------------------------------------------------------------------
			Curve::region_info::region_info(const Vector3D& s, const Vector3D& e, Interpolation::Interpolators f, float* t, float speed) noexcept :
				mInit(s), mEnd(e), mFunc(f), mUserTime(t), mInitTime(*t), mTimer(*t), mSpeed(speed)
			{
			}
			// ------------------------------------------------------------------------
			/*! GetPosition
			*
			*   Gets curve position at moment
			*/ // ---------------------------------------------------------------------
			const Vector3D Curve::GetPosition() const noexcept
			{
				return mCurves.empty() ? 0 : mCurves[mRegion]->mInit + (mCurves[mRegion]->mEnd - mCurves[mRegion]->mInit) * *(mCurves[mRegion]->mUserTime);
			}
#pragma endregion
		}
	}
}