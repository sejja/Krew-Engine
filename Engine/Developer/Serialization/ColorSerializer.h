//
//	ColorSerializer.h
//	Good Neighbours
//
//	Created by Diego Revilla on 11/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _COLOR_SERIALIZER__H_
#define _COLOR_SERIALIZER__H_

namespace Engine {
	namespace Serializers {
		struct ColorSerializer {
#pragma region // Functions
			static nlohmann::json ColorToJson(const Graphics::Color& c) noexcept;
			static Graphics::Color ColorFromJson(const nlohmann::json& j) noexcept;
#pragma endregion
		};
	}
}

#endif