#ifndef _RUNTIME__H_
#define _RUNTIME__H_

namespace Engine {
	enum class cast : unsigned short {
		opt,
		sec,
		def
	};

	// ------------------------------------------------------------------------
	/*! Cast
	*
	*   Casts an object to a new specified object
	*/ // --------------------------------------------------------------------
	template<class Ty_, typename Uy_>
	DONTDISCARD Ty_* Cast(Uy_* what, cast mode = cast::sec) {
		//switch by the conversion mode
		switch (mode) {
		case cast::sec:
			return dynamic_cast<Ty_*>(what);

		case cast::opt:
			return reinterpret_cast<Ty_*>(what);

		case cast::def:
		default:
			return static_cast<Ty_*>(what);
		}
	}
}

#endif