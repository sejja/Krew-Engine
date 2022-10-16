//
//	Conversions.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 03/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Math {
		// ------------------------------------------------------------------------
		/*! Double To String
		*
		*   Converts a Double to a formated String
		*/ // --------------------------------------------------------------------
		std::string Conversions::DoubleToString(const double num) noexcept {
			std::ostringstream _convert;
			_convert << num;

			return std::move(_convert.str());
		}

		// ------------------------------------------------------------------------
		/*! Round Double
		*
		*   Rounds a double value
		*/ // --------------------------------------------------------------------
		constexpr inline double Conversions::RoundDouble(const double n) noexcept {
			return static_cast<float>(static_cast<int>((n * 100.f) + 0.5) / 100);
		}

		// ------------------------------------------------------------------------
		/*! Round Off
		*
		*   Rounds a Double to a String
		*/ // --------------------------------------------------------------------
		std::string Conversions::ConvertSize(unsigned size) noexcept {
			constexpr const char* _sizes[] = { "B", "KB", "MB", "GB" };
			constexpr const unsigned int _kbyte = 1 << 10;
			unsigned char _divcount = 0;
			unsigned _rem = 0;

			//For each size unit
			while (size >= _kbyte) {
				_rem = size % _kbyte;
				_divcount++;
				size /= _kbyte;
			}

			std::string _result = std::move(DoubleToString(RoundDouble((
				static_cast<double>(size) + static_cast<double>(_rem) / _kbyte))));
			_result.reserve(_result.size() + 3);
			_result.append(" "); _result.append(_sizes[_divcount]);

			return std::move(_result);
		}
		glm::mat4 Conversions::ConvertAiMatrixToGLM(const aiMatrix4x4& from) noexcept
		{
			glm::mat4 to;
			to[0][0] = from.a1;
			to[1][0] = from.a2;
			to[2][0] = from.a3;
			to[3][0] = from.a4;
			to[0][1] = from.b1;
			to[1][1] = from.b2;
			to[2][1] = from.b3;
			to[3][1] = from.b4;
			to[0][2] = from.c1;
			to[1][2] = from.c2;
			to[2][2] = from.c3;
			to[3][2] = from.c4;
			to[0][3] = from.d1;
			to[1][3] = from.d2;
			to[2][3] = from.d3;
			to[3][3] = from.d4;
			return to;
		}
	}
}