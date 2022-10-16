#ifndef _SPLINE__H_
#define _SPLINE__H_
namespace Engine
{
	namespace Math
	{
		using namespace Interpolation;

		struct Spline
		{
			Spline();
			Spline& operator=(const Spline& rhs);

			void AddPoint(const glm::vec3& p);
			void SetStart(const bool s);

			bool GetStart() const;
			glm::vec3 GetPointByIndex(unsigned idx) const;
			glm::vec3 GetPos() const;
			bool SplineIsEmpty() const;

			void StartInterpolate();
			void UpdateInterpolate();
			void ResetSpline();

			private:
				std::vector<glm::vec3> mPoints;
				Curve mCurve;
				float mTime;
				bool mStart;
		};

	}
}

#endif