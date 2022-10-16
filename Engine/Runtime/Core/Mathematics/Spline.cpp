#include <Shared.h>

namespace Engine
{
	namespace Math
	{
		using namespace Interpolation;

		Spline::Spline() : mPoints(), mCurve(), mTime(0.0f), mStart(false)
		{

		}

		Spline& Spline::operator=(const Spline& rhs)
		{
			mPoints = std::vector<glm::vec3>(rhs.mPoints.begin(), rhs.mPoints.end());

			mCurve = rhs.mCurve;

			mTime = rhs.mTime;

			return *this;
		}

		void Spline::AddPoint(const glm::vec3& p)
		{
			mPoints.push_back(p);

			if (mPoints.size() == 1)
				return;

			glm::vec3 p0 = mPoints.at(mPoints.size() - 2);
			glm::vec3 p1 = mPoints.at(mPoints.size() - 1);

			mCurve.AddRegion(p0, p1, Interpolators::LINEAR, &mTime);

		}

		void Spline::SetStart(const bool s)
		{
			mStart = s;
		}

		bool Spline::GetStart() const
		{
			return mStart;
		}

		glm::vec3 Spline::GetPointByIndex(unsigned idx) const
		{
			return mPoints[idx];
		}

		bool Spline::SplineIsEmpty() const
		{
			return mPoints.empty();
		}

		void Spline::StartInterpolate()
		{
			if (!mStart)
			{
				mCurve.Play();
				mStart = true;
			}
		}

		void Spline::UpdateInterpolate()
		{
			mCurve.Update();
		}

		void Spline::ResetSpline()
		{
			mCurve.ResetCurve();
		}

		glm::vec3 Spline::GetPos() const
		{
			return mCurve.GetPosition();
		}
	}
}