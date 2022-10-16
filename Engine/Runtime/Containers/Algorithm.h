//
//	Algorithm.h
//	FSESTL
//
//	Created by Diego Revilla on 07/01/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _ALGORITHM__H_
#define _ALGORITHM__H_

namespace Engine {
	// ------------------------------------------------------------------------
	/*! ForEach
	*
	*   Ranged based loop
	*/ //----------------------------------------------------------------------
	template <class ITERATOR, typename FN>
	void FForEach(ITERATOR first, const ITERATOR&& last, FN&& func) noexcept {
		for (; first != last; ++first)
			func(*first);
	}

	// ------------------------------------------------------------------------
	/*! FFind
	*
	*   Searchs in a container for a given value
	*/ //----------------------------------------------------------------------
	template<class ITERATOR, class FN>
	DONTDISCARD ITERATOR FFind(ITERATOR first, const ITERATOR&& last, FN&& val) noexcept {
		while (first != last) {
			if (*first == val) return first;
			++first;
		}
		return last;
	}
}

#endif