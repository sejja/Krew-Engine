//
//	ColorSerializer.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 11/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Serializers {
		namespace {
			constexpr const char* s_common_str[] = { "r", "g", "b", "a" };
			constexpr const size_t s_r_idx = 0, s_g_idx = 1, s_b_idx = 2, s_a_idx = 3;
		}

		// ------------------------------------------------------------------------
		/*! Color To Json
		*
		*   Returns a Json from a Color
		*/ // ---------------------------------------------------------------------
		nlohmann::json ColorSerializer::ColorToJson(const Graphics::Color& col) noexcept {
			nlohmann::json _val;

			_val[s_common_str[s_r_idx]] = col.r, _val[s_common_str[s_g_idx]] = col.g,
				_val[s_common_str[s_b_idx]] = col.b, _val[s_common_str[s_a_idx]] = col.a;

			return std::move(_val);
		}

		// ------------------------------------------------------------------------
		/*! Color From Json
		*
		*   Returns a Color from a Json
		*/ // ---------------------------------------------------------------------
		Graphics::Color ColorSerializer::ColorFromJson(const nlohmann::json& j) noexcept {
			Graphics::Color _col;

			_col.r = j[s_common_str[s_r_idx]].get<float>(),
				_col.g = j[s_common_str[s_g_idx]].get<float>(),
				_col.b = j[s_common_str[s_b_idx]].get<float>(),
				_col.a = j[s_common_str[s_a_idx]].get<float>();

			return std::move(_col);
		}
	}
}