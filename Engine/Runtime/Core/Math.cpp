#include <Shared.h>

namespace Engine {
	namespace Math {
		// ------------------------------------------------------------------------
		/*! Sin
		*
		*   Faster sin than std::sin
		*	Paper: https://perso.univ-perp.fr/hugues.de-lassus/files/publications/productions/TC2703870.pdf
		*/ // ---------------------------------------------------------------------
		float Sin(float x) noexcept {
			int si = int(x * (0.5 * SIN_LOOKUP_SIZE / PI));
			const double d = x - si * (2.0 * PI / SIN_LOOKUP_SIZE);
			int ci = si + SIN_LOOKUP_SIZE / 4;
			si &= SIN_LOOKUP_SIZE - 1;
			ci &= SIN_LOOKUP_SIZE - 1;

			return static_cast<float>(_lookups::sine_table[si] + (_lookups::sine_table[ci] - 0.5 * _lookups::sine_table[si] * d) * d);
		}

		// ------------------------------------------------------------------------
		/*! Cos
		*
		*   Faster cos than std::cos
		*	Paper: https://perso.univ-perp.fr/hugues.de-lassus/files/publications/productions/TC2703870.pdf
		*/ // ---------------------------------------------------------------------
		float Cos(float x) noexcept {
			int ci = int(x * (0.5 * SIN_LOOKUP_SIZE / PI));
			const double d = x - ci * (2.0 * PI / SIN_LOOKUP_SIZE);
			int si = ci + SIN_LOOKUP_SIZE / 4;
			si &= SIN_LOOKUP_SIZE - 1;
			ci &= SIN_LOOKUP_SIZE - 1;

			return static_cast<float>(_lookups::sine_table[si] - (_lookups::sine_table[ci] + 0.5 * _lookups::sine_table[si] * d) * d);
		}
	}
}