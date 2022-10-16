//
//	MemoryTracker.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _DEBUG
namespace Engine {
	namespace Memory {
		// ------------------------------------------------------------------------
		/*! Terminate Memory Debugging
		*
		*   Clears the memory used for Memory Tracking and Checks for Leaks
		*/ // --------------------------------------------------------------------
		void MemoryTracker::TerminateMemoryDebugging() {
			assertThis(mCurrentAllocations.empty(), "Memory Leaks Detected!")
			mBlock.clear();
			mCurrentAllocations.clear();
		}

		// ------------------------------------------------------------------------
		/*! Register Allocation
		*
		*   Register of an Allocation
		*/ // --------------------------------------------------------------------
		void MemoryTracker::RegisterAllocation(const void* mem, const char* fun, size_t bytes) {
			auto& it = mBlock[fun];

			it.mAllocationBytes += bytes;
			it.mAllocationCount++;
			it.mLastAllocationFrame = GFrames->GetFrameCounter();
			mCurrentAllocations[(uintptr_t)mem] = bytes;
		}

		// ------------------------------------------------------------------------
		/*! Register Reallocation
		*
		*   Register of a Reallocation
		*/ // --------------------------------------------------------------------
		void MemoryTracker::RegisterReallocation(const void* mem, const void* newmem, const char* fun, size_t bytes) {
			auto& it = mBlock[fun];

			it.mAllocationBytes += bytes;
			it.mReallocationCount++;
			it.mLastAllocationFrame = GFrames->GetFrameCounter();
			if (mem != newmem) mCurrentAllocations.erase((uintptr_t)mem);
			mCurrentAllocations[(uintptr_t)newmem] = bytes;
		}

		// ------------------------------------------------------------------------
		/*! Register Deallocation
		*
		*   Register of an Deallocation
		*/ // --------------------------------------------------------------------
		void MemoryTracker::RegisterDeallocation(const void* mem, const char* fun) {
			if (mCurrentAllocations.size()) {
				std::map<std::string, MemoryInfo>::iterator it = mBlock.find(fun);

				if (it != mBlock.end())
					it->second.mDeallocationCount++;

				mCurrentAllocations.erase((uintptr_t)mem);
			}
		}
	}
}
#endif