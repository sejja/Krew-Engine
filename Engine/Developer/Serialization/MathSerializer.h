//
//	MathSerializer.h
//	Good Neighbours
//
//	Created by Diego Revilla on 23/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _MATH_SERIALIZER__H_
#define _MATH_SERIALIZER__H_

namespace Engine {
	namespace Serializers {
		struct MathSerializer {
#pragma region //Functions
			static nlohmann::json Vector3DToJson(const Math::Vector3D& vec) noexcept;
			static Math::Vector3D Vector3DFromJson(const nlohmann::json& j) noexcept;
			static nlohmann::json Vector2DToJson(const Math::Vector2D& vec) noexcept;
			static Math::Vector2D Vector2DFromJson(const nlohmann::json& j) noexcept;
#pragma endregion
		};
	}
}

#endif