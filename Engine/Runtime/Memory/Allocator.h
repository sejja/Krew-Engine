//
//	Allocator.h
//	FSESTL
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _ALLOCATOR__H_
#define _ALLOCATOR__H_

namespace Engine {
	template<typename TYPE>
	struct Allocator {
#if _DEBUG && __cpp_consteval
		DONTDISCARD constexpr static TYPE inline* allocate(const size_t size = 1,
			const Engine::CodeTracker& loc = Engine::CodeTracker::Current()) noexcept;
		DONTDISCARD static constexpr TYPE inline* reallocate(TYPE* mem, const size_t size = 1,
			const Engine::CodeTracker& loc = Engine::CodeTracker::Current()) noexcept;
		static void inline deallocate(TYPE* data,
			const Engine::CodeTracker& loc = Engine::CodeTracker::Current()) noexcept;
		constexpr static TYPE inline* New(size_t count = 1, const Engine::CodeTracker& loc = Engine::CodeTracker::Current()) noexcept;
#else
		DONTDISCARD constexpr static TYPE inline* allocate(const size_t size = 1) noexcept;
		DONTDISCARD constexpr static TYPE inline* reallocate(TYPE* mem, const size_t size = 1) noexcept;
		static void inline deallocate(TYPE* data) noexcept;
		constexpr static TYPE inline* New(size_t count = 1) noexcept;
#endif

		template<typename ...Args>
		static void inline construct(TYPE* data, Args ...ag) noexcept;
		static void inline destroy(const TYPE* data) noexcept;
	};

#if _DEBUG && __cpp_consteval
	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   Allocates sizeof(TYPE) * size bytes
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* Allocator<TYPE>::allocate(const size_t size, const Engine::CodeTracker& loc) noexcept {
		TYPE* _temp = reinterpret_cast<TYPE*>(malloc(size * sizeof(TYPE)));

		GMemoryDbg->RegisterAllocation(_temp, (std::string(loc.File()) + " - " + std::string(loc.Function())).c_str(), size * sizeof(TYPE));
		return _temp;
	}

	// ------------------------------------------------------------------------
	/*! Reallocate
	*
	*   Reallocates an ammount of memory extending it's size
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* Allocator<TYPE>::reallocate(TYPE* mem, const size_t size, const Engine::CodeTracker& loc)
		noexcept {
		TYPE* _temp = reinterpret_cast<TYPE*>(realloc(mem, size * sizeof(TYPE)));

		GMemoryDbg->RegisterReallocation(mem, _temp, (std::string(loc.File()) + " - " + std::string(loc.Function())).c_str(), size * sizeof(TYPE));
		return _temp;
	}

	// ------------------------------------------------------------------------
	/*! Deallocate
	*
	*   Deallocates the passed chunk
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	void inline Allocator<TYPE>::deallocate(TYPE* data, const Engine::CodeTracker& loc) noexcept {
		GMemoryDbg->RegisterDeallocation(data, (std::string(loc.File()) + " - " + std::string(loc.Function())).c_str());
		free(data);
	}

	template<typename TYPE>
	constexpr TYPE inline* Allocator<TYPE>::New(size_t count, const Engine::CodeTracker& loc) noexcept {
		TYPE* temp_ = allocate(count, loc);
		construct(temp_);
		return temp_;
	}
#else
	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   Allocates sizeof(TYPE) * size bytes
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* Allocator<TYPE>::allocate(const size_t size) noexcept {
		return reinterpret_cast<TYPE*>(malloc(size * sizeof(TYPE)));
	}

	// ------------------------------------------------------------------------
	/*! Reallocate
	*
	*   Reallocates an ammount of memory extending it's size
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* Allocator<TYPE>::reallocate(TYPE* mem, const size_t size) noexcept {
		return reinterpret_cast<TYPE*>(realloc(mem, size * sizeof(TYPE)));
	}

	// ------------------------------------------------------------------------
	/*! Deallocate
	*
	*   Deallocates the passed chunk
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	void inline Allocator<TYPE>::deallocate(TYPE* data) noexcept {
		free(data);
	}

	template<typename TYPE>
	constexpr TYPE inline* Allocator<TYPE>::New(size_t count) noexcept {
		TYPE* temp_ = allocate(count);

		FForEach(temp_, temp_ + count, [](TYPE& c) {construct(&c); });
		return temp_;
	}
#endif
	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   construct a variable of type TYPE
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	template<typename ...Args>
	void inline Allocator<TYPE>::construct(TYPE* data, Args ...ag) noexcept {
		new(data) TYPE(ag...);
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Calls the TYPE destructor
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	void inline Allocator<TYPE>::destroy(const TYPE* data) noexcept {
		data->~TYPE();
	}
}

#endif