//
//	Transform.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _TRANSFORM__H_
#define _TRANSFORM__H_

namespace Engine {
	namespace Math {
		struct ENGINE_API Transform : public Serializable {
#pragma region // Constructors & Destructors
			Transform() noexcept;
			Transform(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale) noexcept;
#pragma endregion

#pragma region // Functions
			DONTDISCARD glm::mat4 GetModelMtx() const noexcept;
			DONTDISCARD glm::mat4 GetModelMtx_noScale() const noexcept;
			void FromJson(const nlohmann::json& val) noexcept override;
			void ToJson(nlohmann::json& val) const noexcept override;
			void SetIdentity() noexcept;
			DONTDISCARD glm::mat4 GetInverseMatrix() const noexcept;
			DONTDISCARD glm::mat4 GetInverseMatrix_noScale() const noexcept;
			DONTDISCARD Transform InverseConcatenation(const Transform& rhs) const noexcept;
			DONTDISCARD Vector3D MultiplyPoint(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D MultiplyDirection(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D InverseMultiplyPoint(const Vector3D& rhs) const noexcept;
			DONTDISCARD Vector3D InverseMultiplyDirection(const Vector3D& rhs) const noexcept;
#pragma endregion

#pragma region // Members
		public:
			Vector3D mPos, mScale, mRot;
#pragma endregion
		};

		bool ENGINE_API fast_decompose_transfom(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	}
}

#endif