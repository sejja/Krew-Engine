//
//	Vector.h
//	Engine
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _VECTOR__H_
#define _VECTOR__H_

namespace Engine {
	namespace Math {
		struct ENGINE_API Vector2D
		{
#pragma region // Declarations
			float x, y;
#pragma endregion

#pragma region // Constructors & Destructors
			Vector2D(const float _x = 0.0f, const float _y = 0.0f) noexcept;
			Vector2D(const glm::vec2& vec) noexcept;
			Vector2D(const Vector2D& vec) noexcept;
#pragma endregion

#pragma region // Methods
			DONTDISCARD inline operator glm::vec2() const noexcept;
			DONTDISCARD Vector2D inline operator+(const Vector2D& rhs) const noexcept;
			DONTDISCARD Vector2D inline operator-(const Vector2D& rhs) const noexcept;
			DONTDISCARD Vector2D inline operator-() const noexcept;
			DONTDISCARD float  inline operator*(const Vector2D& rhs) const noexcept;
			DONTDISCARD Vector2D inline operator*(const float rhs) const noexcept;
			DONTDISCARD Vector2D inline operator/(const float rhs) const noexcept;
			Vector2D& operator+=(const Vector2D& rhs) noexcept;
			Vector2D& operator-=(const Vector2D& rhs) noexcept;
			Vector2D& operator*=(const float rhs) noexcept;
			Vector2D& operator/=(const float rhs) noexcept;
			DONTDISCARD bool inline operator==(const Vector2D& rhs) const noexcept;
			DONTDISCARD bool inline operator!=(const Vector2D& rhs) const noexcept;
			DONTDISCARD float Length() const noexcept;
			DONTDISCARD Vector2D Normalize() const noexcept;
			float	DistanceSquare(const Vector2D& rhs);
#pragma endregion
		};

		struct ENGINE_API Vector3D
		{
#pragma region // Declarations
			float x, y, z;
#pragma endregion

#pragma region // Constructors & Destructors
			Vector3D(const float _x = 0.0f, const float _y = 0.0f, const float _z = 0.0f) noexcept;
			Vector3D(const glm::vec3& vec) noexcept;
			Vector3D(const Vector3D& vec) noexcept;
#pragma endregion

#pragma region // Methods
			DONTDISCARD inline operator glm::vec3() const noexcept;
			DONTDISCARD Vector3D inline operator+(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D inline operator-(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D inline operator-() const noexcept;
			DONTDISCARD float inline operator*(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D inline operator*(const float rhs) const noexcept;
			DONTDISCARD Vector3D inline operator/(const float rhs) const noexcept;
			const Vector3D& operator+=(const Vector3D& rhs) noexcept;
			const Vector3D& operator-=(const Vector3D& rhs) noexcept;
			const Vector3D& operator*=(const float rhs) noexcept;
			const Vector3D& operator/=(const float rhs) noexcept;
			DONTDISCARD bool inline operator==(const Vector3D& rhs) const noexcept;
			DONTDISCARD bool inline operator!=(const Vector3D& rhs) const noexcept;
			DONTDISCARD bool inline operator<=(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D inline Cross(const Vector3D& rhs) const noexcept;
			DONTDISCARD float Length() const noexcept;
			DONTDISCARD Vector3D Normalize() const noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Operator glm::vec2
		*
		*   Casts a Vector2D into a glm::vec2
		*/ // ---------------------------------------------------------------------
		Vector2D::operator glm::vec2() const noexcept {
			return { x, y };
		}

		// ------------------------------------------------------------------------
		/*! Operator+
		*
		*   Adds two Vectors2D
		*/ // ---------------------------------------------------------------------
		Vector2D Vector2D::operator+(const Vector2D& rhs) const noexcept
		{
			return { x + rhs.x, y + rhs.y };
		}

		// ------------------------------------------------------------------------
		/*! Operator-
		*
		*   Substracts two Vectors2D
		*/ // ---------------------------------------------------------------------
		Vector2D Vector2D::operator-(const Vector2D& rhs) const noexcept
		{
			return { x - rhs.x, y - rhs.y };
		}

		// ------------------------------------------------------------------------
		/*! Operator-()
		*
		*   Negates a Vector2D
		*/ // ---------------------------------------------------------------------
		Vector2D Vector2D::operator-() const noexcept
		{
			return { -x, -y };
		}

		// ------------------------------------------------------------------------
		/*! Operator*
		*
		*   Dot Product
		*/ // ---------------------------------------------------------------------
		float Vector2D::operator*(const Vector2D& rhs) const noexcept
		{
			// Dot product
			return x * rhs.x + y * rhs.y;
		}

		// ------------------------------------------------------------------------
		/*! Operator*
		*
		*   Multiplies a Vector2D By a float
		*/ // ---------------------------------------------------------------------
		Vector2D Vector2D::operator*(const float rhs) const noexcept
		{
			return { x * rhs, y * rhs };
		}

		// ------------------------------------------------------------------------
		/*! Operator/
		*
		*   Divides a Vector2D By a float
		*/ // ---------------------------------------------------------------------
		Vector2D Vector2D::operator/(const float rhs) const noexcept
		{
			return Vector2D(x / rhs, y / rhs);
		}

		// ------------------------------------------------------------------------
		/*! Operator==
		*
		*   Checks if two Vector2D are equal
		*/ // ---------------------------------------------------------------------
		bool Vector2D::operator==(const Vector2D& rhs) const noexcept
		{
			return x == rhs.x && y == rhs.y;
		}

		// ------------------------------------------------------------------------
		/*! Operator!=
		*
		*   Checks if two Vector2D are not equal
		*/ // ---------------------------------------------------------------------
		bool Vector2D::operator!=(const Vector2D& rhs) const noexcept
		{
			return !(*this == rhs);
		}

		// ------------------------------------------------------------------------
		/*! Operator glm::vec3
		*
		*   Casts a Vector3D into a glm::vec3
		*/ // ---------------------------------------------------------------------
		Vector3D::operator glm::vec3() const noexcept {
			return { x, y, z };
		}

		// ------------------------------------------------------------------------
		/*! Operator+
		*
		*   Adds two Vectors3D
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::operator+(const Vector3D& rhs) const noexcept
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		// ------------------------------------------------------------------------
		/*! Operator-
		*
		*   Substracts two Vectors3D
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::operator-(const Vector3D& rhs) const noexcept
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		// ------------------------------------------------------------------------
		/*! Operator-()
		*
		*   Negates a Vector3D
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::operator-() const noexcept
		{
			return { -x, -y, -z };
		}

		// ------------------------------------------------------------------------
		/*! Operator*
		*
		*   Dot Product
		*/ // ---------------------------------------------------------------------
		float Vector3D::operator*(const Vector3D& rhs) const noexcept
		{
			// Dot product
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		// ------------------------------------------------------------------------
		/*! Operator*
		*
		*   Multiplies a Vector3D By a float
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::operator*(const float rhs) const noexcept
		{
			return { x * rhs, y * rhs, z * rhs };
		}

		// ------------------------------------------------------------------------
		/*! Operator/
		*
		*   Divides a Vector3D By a float
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::operator/(const float rhs) const noexcept
		{
			return { x / rhs, y / rhs, z / rhs };
		}

		// ------------------------------------------------------------------------
		/*! Operator==
		*
		*   Checks if two Vector3D are equal
		*/ // ---------------------------------------------------------------------
		bool Vector3D::operator==(const Vector3D& rhs) const noexcept
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}

		// ------------------------------------------------------------------------
		/*! Operator!=
		*
		*   Checks if two Vector3D are not equal
		*/ // ---------------------------------------------------------------------
		bool Vector3D::operator!=(const Vector3D& rhs) const noexcept
		{
			return !(*this == rhs);
		}

		bool Vector3D::operator<=(const Vector3D& rhs) const noexcept
		{
			return (x*x + y*y + z*z) <= (rhs.x * rhs.x + rhs.y * rhs.y + rhs.z * rhs.z);
		}

		// ------------------------------------------------------------------------
		/*! Cross
		*
		*   Cross two Vector3D
		*/ // ---------------------------------------------------------------------
		Vector3D Vector3D::Cross(const Vector3D& rhs) const noexcept
		{
			// Compute the cross product and return it
			return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x };
		}
	}
}

#endif