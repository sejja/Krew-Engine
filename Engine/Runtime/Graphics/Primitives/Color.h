//
//	Color.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _COLOR__H_
#define _COLOR__H_

namespace Engine {
	namespace Graphics {
		struct ENGINE_API Color
		{
#pragma region // Constructors & Destructors
			Color(const float _r = 0.f, const float _g = 0.f, const float _b = 0.f, const float _a = 1.f) noexcept;
			Color(const glm::vec4& col) noexcept;
#pragma endregion

			DONTDISCARD inline operator glm::vec4() const noexcept;

#pragma region // Members
			union
			{
				float v[4];
				struct
				{
					float x, y, z, w;
				};
				struct
				{
					float r, g, b, a;
				};
			};

#pragma endregion
		};

		inline Color::operator glm::vec4() const noexcept
		{
			return glm::vec4(r, g, b, a);
		}

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Color, r, g, b, a)
	}
}

#endif