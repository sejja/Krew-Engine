//
//	MemoryTracker.h
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _DEBUG
#ifndef _MEMORY_TRACKER__H_
#define _MEMORY_TRACKER__H_

#ifdef _EDITOR_
namespace Editor {
	class MemoryTrackerWindow;
}
#endif

namespace Engine {
	namespace Memory {
		class MemoryTracker : public Singleton<MemoryTracker> {
#ifdef _EDITOR_
			friend Editor::MemoryTrackerWindow;
#endif
		public:
			void RegisterAllocation(const void* mem, const char* fun, size_t bytes);
			void RegisterReallocation(const void* mem, const void* newmem, const char* fun, size_t bytes);
			void RegisterDeallocation(const void* mem, const char* fun);
			void TerminateMemoryDebugging();
		private:
			struct MemoryInfo {
				size_t mAllocationCount = 0;
				size_t mAllocationBytes = 0;
				size_t mLastAllocationFrame = 0;
				size_t mReallocationCount = 0;
				size_t mDeallocationCount = 0;
			};

			std::map<std::string, MemoryInfo> mBlock; // Should be faster with a Hash-Map, but it's not...
																		  // will leave it like this for now.
																		  // Need to investigate this further, if time allows
			std::map<uintptr_t, size_t> mCurrentAllocations;
		};
	}
}

#define GMemoryDbg (&Engine::Memory::MemoryTracker::Instance())

#endif
#endif