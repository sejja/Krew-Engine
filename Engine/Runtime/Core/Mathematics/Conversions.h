//
//	Conversions.h
//	Good Neighbours
//
//	Created by Diego Revilla on 03/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _CONVERSIONS__H_
#define _CONVERSIONS__H_

namespace Engine {
	namespace Math {
		struct Conversions {
			DONTDISCARD static std::string DoubleToString(const double num) noexcept;
			DONTDISCARD static constexpr inline double RoundDouble(const double n) noexcept;
			DONTDISCARD static std::string ConvertSize(unsigned size) noexcept;
#ifdef _ENGINE_
			DONTDISCARD static glm::mat4 ConvertAiMatrixToGLM(const aiMatrix4x4& from) noexcept;
#endif
		};
	}
}

#endif