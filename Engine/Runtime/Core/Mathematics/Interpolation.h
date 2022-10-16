//
//	Interpolation.h
//	Good Neighbours
//
//	Created by Adrian Montes on 22/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _INTERPOLATION_H_
#define _INTERPOLATION_H_
#define FRAMERATE 0.016f

namespace Engine
{
	namespace Math
	{
		namespace Interpolation
		{
#pragma region // Interpolation Functions
			enum class Interpolators
			{
				LINEAR,	IN_QUAD, OUT_QUAD, IN_OUT_QUAD
			};
			float EaseLinear(float t) noexcept;
			float EaseInQuad(float t) noexcept;
			float EaseOutQuad(float t) noexcept;
			float EaseInOutQuad(float t) noexcept;
#pragma endregion
			class ENGINE_API Curve
			{
				struct region_info
				{
					Vector3D mInit;
					Vector3D mEnd;
					Interpolation::Interpolators mFunc;
					float* mUserTime;
					float mTimer;
					float mInitTime;
					float mSpeed;

					region_info(const Vector3D& s, const Vector3D& e, Interpolation::Interpolators f, float* t, float speed) noexcept;
				};
#pragma region // Members
				bool mIsInterpolating;
				bool mLoop;
				unsigned mRegion;
				std::vector<region_info*> mCurves;
#pragma endregion
			public:

#pragma region // Constructors and Destructors
				Curve() noexcept;
				~Curve() noexcept;
#pragma endregion
#pragma region // Functions
				void AddRegion(const Vector3D& start, const Vector3D& end, Interpolation::Interpolators type, float* t, float speed = 1.0f) noexcept;
				void RemoveRegion(unsigned r) noexcept;
				void SetLoop(bool l) noexcept;
				void Play() noexcept;
				void Pause() noexcept;
				void Update() noexcept;
				Curve& operator=(const Curve& rhs);
				const Vector3D GetPosition() const noexcept;
				void ResetCurve() noexcept;
				bool CurveIsEmpty() const noexcept;
#pragma endregion

#pragma region // Gettors
				bool IsInterpolating() const noexcept;
				bool IsLooping() const noexcept;
				unsigned GetRegion() const noexcept;
#pragma endregion
			};

		}
	}
}
#endif // !_INTERPOLATION_H_

