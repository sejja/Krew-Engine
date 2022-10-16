//
//	Vector.cpp
//	Engine
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include "Shared.h"

namespace Engine {
	namespace Math {
		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Vector2D from a couple of floats
		*/ // ---------------------------------------------------------------------
		Vector2D::Vector2D(const float _x, const float _y) noexcept
			: x(_x), y(_y) {
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Converts a glm::vec2 into a Vector2D
		*/ // ---------------------------------------------------------------------
		Vector2D::Vector2D(const glm::vec2& vec) noexcept :
			x(vec.x), y(vec.y) {
		}

		// ------------------------------------------------------------------------
		/*! Copy Constructor
		*
		*   Duplicates a Vector2D
		*/ // ---------------------------------------------------------------------
		Vector2D::Vector2D(const Vector2D& vec) noexcept :
			x(vec.x), y(vec.y) {
		}

		// ------------------------------------------------------------------------
		/*! Operator+=
		*
		*   Add concatenation of two Vector2D
		*/ // ---------------------------------------------------------------------
		Vector2D& Vector2D::operator+=(const Vector2D& rhs) noexcept
		{
			x += rhs.x, y += rhs.y;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator-=
		*
		*   Substract concatenatio of two Vector2D
		*/ // ---------------------------------------------------------------------
		Vector2D& Vector2D::operator-=(const Vector2D& rhs) noexcept
		{
			x -= rhs.x, y -= rhs.y;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator*=
		*
		*   Multiplication concatenation of a Vector2D and a float
		*/ // ---------------------------------------------------------------------
		Vector2D& Vector2D::operator*=(const float rhs) noexcept
		{
			x *= rhs, y *= rhs;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator/=
		*
		*   Division concatenation of a Vector2D and a float
		*/ // ---------------------------------------------------------------------
		Vector2D& Vector2D::operator/=(const float rhs) noexcept
		{
			x /= rhs, y /= rhs;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Length
		*
		*   Gets the Length of a vector
		*/ // ---------------------------------------------------------------------
		float Vector2D::Length() const noexcept
		{
			return Engine::Math::SquareRoot(x * x + y * y);
		}

		// ------------------------------------------------------------------------
		/*! Normalize
		*
		*   Normalizes a Vector2D
		*/ // ---------------------------------------------------------------------
		Vector2D Vector2D::Normalize() const noexcept
		{
			const float _length = Length();

			return _length ? Vector2D(x / _length, y / _length) : Vector2D();
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Vector3D from a tuple of floats
		*/ // ---------------------------------------------------------------------
		Vector3D::Vector3D(const float _x, const float _y, const float _z) noexcept
			: x(_x), y(_y), z(_z) {
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Converts a glm::vec3 into a Vector3D
		*/ // ---------------------------------------------------------------------
		Vector3D::Vector3D(const glm::vec3& vec) noexcept
			: x(vec.x), y(vec.y), z(vec.z) {
		}

		// ------------------------------------------------------------------------
		/*! Copy Constructor
		*
		*   Duplicates a Vector3D
		*/ // ---------------------------------------------------------------------
		Vector3D::Vector3D(const Vector3D& vec) noexcept
			: x(vec.x), y(vec.y), z(vec.z) {
		}

		// ------------------------------------------------------------------------
		/*! Operator+=
		*
		*   Add concatenation of two Vector3D
		*/ // ---------------------------------------------------------------------
		const Vector3D& Vector3D::operator+=(const Vector3D& rhs) noexcept
		{
			x += rhs.x, y += rhs.y, z += rhs.z;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator-=
		*
		*   Substraction concatenation of two Vector3D
		*/ // ---------------------------------------------------------------------
		const Vector3D& Vector3D::operator-=(const Vector3D& rhs) noexcept
		{
			x -= rhs.x, y -= rhs.y, z -= rhs.z;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator*=
		*
		*   Multiply concatenation of a Vector3D with a float
		*/ // ---------------------------------------------------------------------
		const Vector3D& Vector3D::operator*=(const float rhs) noexcept
		{
			x *= rhs, y *= rhs, z *= rhs;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator/=
		*
		*   Divide concatenation of a Vector3D and a float
		*/ // ---------------------------------------------------------------------
		const Vector3D& Vector3D::operator/=(const float rhs) noexcept
		{
			x /= rhs, y /= rhs, z /= rhs;

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Length
		*
		*   Gets the length of the vector
		*/ // ---------------------------------------------------------------------
		float Vector3D::Length() const noexcept
		{
			return Engine::Math::SquareRoot(x * x + y * y + z * z);
		}

		// ------------------------------------------------------------------------
		/*! Normalize
		*
		*   Normalizes a Vector3D
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::Normalize() const noexcept
		{
			const float _length = Length();

			return _length ? Vector3D{ x / _length, y / _length, z / _length } : Vector3D();
		}

		float	Vector2D::DistanceSquare(const Vector2D& rhs) {
			//return
			return (pow(x - rhs.x, 2) + pow(y - rhs.y, 2));
		}
	}
}