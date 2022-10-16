//
//	DrawingFunctions.h
//	Good Neighbours
//
//	Created by Diego Revilla on 25/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _DRAWING_FUNCTIONS__H_
#define _DRAWING_FUNCTIONS__H_

namespace Editor {
	class ENGINE_API UIPropertyDraw {
	public:
#pragma region // Declarations
		using text_t = DiffPtr<const char>;
#pragma endregion

#pragma region // Functions
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<Engine::Math::Vector3D>>& info) noexcept;
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<int>>& info) noexcept;
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<float>>& info) noexcept;
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<unsigned>>& info) noexcept;
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<bool>>& info) const noexcept;
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<std::string>>& info) const noexcept;
		DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<Engine::Graphics::Color>>& info) noexcept;
		DONTDISCARD bool operator<<(const std::tuple<text_t, DiffPtr<int>, text_t>& info) noexcept;
		DONTDISCARD bool operator<<(const std::tuple<text_t, DiffPtr<Engine::Graphics::Color>, unsigned char>& info) noexcept;
		DONTDISCARD bool operator<<=(const std::tuple<text_t, text_t, DiffPtr<std::string>>& info) const noexcept;
		//DONTDISCARD bool operator<<(const std::pair<text_t, DiffPtr<Behavior*>>& info) noexcept;

#ifdef __cpp_consteval
		template<typename T>
		DONTDISCARD consteval bool inline operator<<(const std::pair<text_t, const T&>& info) noexcept {
			return false;
		}
#else
		template<typename T>
		DONTDISCARD constexpr bool inline operator<<(const std::pair<text_t, const T&>& info) const noexcept {
			return false;
		}
#endif
#pragma endregion
	};
}

#endif
#endif