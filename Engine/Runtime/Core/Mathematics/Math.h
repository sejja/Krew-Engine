//
//	Math.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _MATH__H_
#define _MATH__H_

namespace Engine {
	namespace Math {

		// ------------------------------------------------------------------------
	/*! Absolute
	*
	*   Returns the absolute value of wathever
	*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD Ty_ inline Absolute(const Ty_ f) noexcept {
			return f > -f ? f : -f;
		}

		// ------------------------------------------------------------------------
		/*! Approx
		*
		*   Returns wherever a value is close enough to another value
		*/ // --------------------------------------------------------------------
		template<typename Ty_, typename Uy_>
		DONTDISCARD bool inline Approx(const Ty_ a, const Ty_ b, const Uy_ n) noexcept {
			return (a - b >= n) && (a - b <= n);
		}

		// ------------------------------------------------------------------------
		/*! Min
		*
		*   Returns smallest float
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD Ty_ inline Min(const Ty_ a, const Ty_ b) noexcept {
			return a < b ? a : b;
		}

		// ------------------------------------------------------------------------
		/*! Max
		*
		*   Returns the biggest float
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD Ty_ inline Max(const Ty_ a, const Ty_ b) noexcept {
			return a > b ? a : b;
		}

		DONTDISCARD glm::vec3 inline Project(glm::vec3 vec1, glm::vec3 vec2) {
			//performs the formula of the projection of the vector into rhs
			glm::vec3 result = vec2 * (glm::dot(vec1, vec2)) / (vec2.x * vec2.x + vec2.y * vec2.y);
			return result;
		}

		DONTDISCARD glm::mat3 inline Rotate(float angle_rad)
		{
			//Set the rotation matrix (explained in class
			return glm::mat3(cos(angle_rad), -sin(angle_rad), 0.0f, sin(angle_rad), cos(angle_rad), 0.0f, 0.0f, 0.0f, 1.0f);
		}

		// ------------------------------------------------------------------------
		/*! Remap
		*
		*   Remaps a value between two values into another value comprended into
		*	the same Transform.Position of other two values
		*/ // --------------------------------------------------------------------
		template<typename Ty_, typename Uy_>
		DONTDISCARD auto inline Remap(const Ty_ a, const Ty_ b, const Ty_ c, const Ty_ d,
			const Uy_ e) noexcept -> decltype(a* e) {
			return (static_cast<Uy_>((a - b) / (c - b)) * (e - d)) + d;
		}

		// ------------------------------------------------------------------------
		/*! Lerp
		*
		*   Returns the Linear Interpolate of a Float
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD auto inline Lerp(const Ty_& a, const Ty_& b, const float f) noexcept ->
			decltype(a* f) {
			return static_cast<Ty_>(a + (b - a) * f);
		}

		// ------------------------------------------------------------------------
		/*! Middle
		*
		*   Returns the middle value of two values
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD Ty_ inline Middle(const Ty_ a, const Ty_ b) noexcept {
			return (static_cast<Ty_>((b - a) * .5f)) + a;
		}

		// ------------------------------------------------------------------------
		/*! Square Root
		*
		*   Calculate the Square Root using Compiler Intrisics (double)
		*/ // ---------------------------------------------------------------------
#if _WIN64
		DONTDISCARD double inline __fastcall SquareRoot(double n) {
			__m128d temp = _mm_set_sd(n);

			temp = _mm_sqrt_sd(temp, temp);

			return _mm_cvtsd_f64(temp);
#else
		DONTDISCARD double inline CATRIBUTE __fastcall SquareRoot(double n) {
			_asm {
				fld qword ptr[esp + 4]
				fsqrt
				ret 8
			}
#endif
		}

		// ------------------------------------------------------------------------
		/*! Square Root
		*
		*   Calculate the Square Root using Compiler Intrisics (float)
		*/ // ---------------------------------------------------------------------
		DONTDISCARD inline float _fastcall SquareRoot(float n) {
#ifdef _WIN64
			__m128 temp = _mm_set_ss(n);

			temp = _mm_sqrt_ss(temp);

			return _mm_cvtss_f32(temp);
#else
			__asm {
				fld n
				fsqrt
			}
#endif
		}

		DONTDISCARD float ENGINE_API Sin(float x) noexcept;
		DONTDISCARD float ENGINE_API Cos(float x) noexcept;
		// ------------------------------------------------------------------------
		/*! Pow
		*
		*   Faster pow than std::pow
		*	Source: https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
		*/ // ---------------------------------------------------------------------
		template<typename T>
		DONTDISCARD constexpr T inline Pow(T a, T b) noexcept {
			union {
				double d;
				int x[2];
			} u = { static_cast<double>(a) };

			u.x[1] = static_cast<int>(static_cast<double>(b) * (u.x[1] - 1072632447) + 1072632447);
			u.x[0] = 0;

			return static_cast<T>(u.d);
		}

		// ------------------------------------------------------------------------
		/*! Asin
		*
		*   Faster asin than std::asin
		*/ // ---------------------------------------------------------------------
		template<typename T>
		DONTDISCARD T inline ASin(T x) noexcept {
			return Pow(Sin(x), static_cast<T>(-1));
		}

		// ------------------------------------------------------------------------
		/*! Acos
		*
		*   Faster acos than std::acos
		*/ // ---------------------------------------------------------------------
		template<typename T>
		DONTDISCARD T inline ACos(T x) noexcept {
			return Pow(Cos(x), static_cast<T>(-1));
		}

		template<class Ty>
		DONTDISCARD constexpr const Ty Clamp(const Ty & val, const Ty & lo, const Ty & hi) noexcept {
			auto clamped_value = val;
			if (val < lo) clamped_value = lo;
			else if (val > hi) clamped_value = hi;

			return clamped_value;
		}
		}
	}

#endif