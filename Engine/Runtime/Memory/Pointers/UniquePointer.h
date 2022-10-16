//
//  UniquePointer.h
//  Good Neightbours
//
//  Created by Diego Revilla on 30/11/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifndef _UNIQUE_PTR__H_
#define _UNIQUE_PTR__H_

namespace Engine {
	template<typename T>
	class UniquePointer {
	public:
		using value_type = T;
		using pointer_type = value_type*;
		using type_ref = T&;
		using unique_ptr = UniquePointer<value_type>;

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*  Sets the Default pointer value to null
		*/ // ---------------------------------------------------------------------
		UniquePointer() :
			mPtr(nullptr) {}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Owns the passed pointer
		*/ // --------------------------------------------------------------------
		UniquePointer(pointer_type ptr) :
			mPtr(ptr) {}

		// ------------------------------------------------------------------------
		/*! Move Constructor
		*
		*   Owns the object of another unique pointer
		*/ // --------------------------------------------------------------------
		UniquePointer(unique_ptr&& other) {
			_swap(other.mPtr);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Destroys the pointee
		*/ // --------------------------------------------------------------------
		~UniquePointer() {
			//If we have a valid pointer
			if (mPtr)
				_clean();
		}

		// ------------------------------------------------------------------------
		/*! Operator=
		*
		*   Gets ownership of an external pointer and destroys the old one
		*/ // --------------------------------------------------------------------
		type_ref operator=(pointer_type rhs) {
			_clean();
			mPtr = rhs;

			return *mPtr;
		}

		// ------------------------------------------------------------------------
		/*! Operator =
		*
		*   Owns the object of another moved unique pointer
		*/ // --------------------------------------------------------------------
		unique_ptr& operator=(unique_ptr&& other) {
			_clean();
			_swap(other.mPtr);

			return *this;
		}

		// ------------------------------------------------------------------------
		/*! Operator!
		*
		*   Returns wether the operator is not valid
		*/ // --------------------------------------------------------------------
		bool operator!() {
			return !mPtr;
		}

		// ------------------------------------------------------------------------
		/*! Operator==
		*
		*  Checks wether an external raw pointer equals the wrapped pointer
		*/ // ---------------------------------------------------------------------
		bool inline operator==(pointer_type rhs) const {
			return mPtr == rhs;
		}

		// ------------------------------------------------------------------------
		/*! Operator!=
		*
		*  Checks wether an external raw pointer doesn't equal the wrapped pointer
		*/ // ---------------------------------------------------------------------
		bool inline operator!=(pointer_type rhs) const {
			return mPtr != rhs;
		}

		// ------------------------------------------------------------------------
		/*! Operator*
		*
		*  Returns a Dereference to the wrapped pointer
		*/ // ---------------------------------------------------------------------
		type_ref inline operator*() const {
			return *mPtr;
		}

		// ------------------------------------------------------------------------
		/*! Operator ->
		*
		*  Treturns the wrapped pointer on a protedted way
		*/ // ---------------------------------------------------------------------
		pointer_type inline operator->() const {
			return mPtr;
		}

		// ------------------------------------------------------------------------
		/*! Get
		*
		*  Returns the wrapped pointer
		*/ // ---------------------------------------------------------------------
		pointer_type inline get() const {
			return mPtr;
		}

		UniquePointer(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

	private:

		// ------------------------------------------------------------------------
		/*! Clean
		*
		*  Deletes the pointee with the preferred method
		*/ // ---------------------------------------------------------------------
		void _clean() {
			Allocator<T>::destroy(mPtr);
			Allocator<T>::deallocate(mPtr);
		}

		// ------------------------------------------------------------------------
		/*! Swap
		*
		*   Swaps the inner pointer with the new one and makes it unique
		*/ // --------------------------------------------------------------------
		void _swap(pointer_type& newptr) {
			mPtr = newptr;
			newptr = nullptr;
		}

		pointer_type mPtr;
	};

	// ------------------------------------------------------------------------
	/*! Make Shared
	*
	*  Returns a new Shared Pointer
	*/ // ---------------------------------------------------------------------
	template<typename T, typename... Args>
	UniquePointer<T> make_unique_ptr(Args&& ... args) {
		return UniquePointer<T>(new T(std::forward<Args>(args)...));
	}
}

#endif