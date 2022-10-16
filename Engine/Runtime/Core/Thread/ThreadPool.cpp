#include <Shared.h>

namespace Engine {
	namespace Threading {
		ThreadPool::ThreadPool(unsigned n)
			: mSize(n) {
			mThreadPool.reserve(mSize);

			for (unsigned i = 0; i != mSize; ++i) {
				mThreadPool.emplace_back(std::thread(&ThreadPool::UpdatePool, this));
				mThreadPool.back().detach();
			}
		}

		ThreadPool::~ThreadPool() {
			for (unsigned th = 0; th != mSize; ++th) {
				if (mThreadPool[th].joinable())
					mThreadPool[th].join();
			}
		}

		void ThreadPool::Resize(unsigned n) {
			assertThis(n != 0, "A Thread Pool of 0 threads?");

			if (n > mSize) {
				unsigned int Temp = mSize;
				mSize = n;
				mThreadPool.resize(mSize);

				for (unsigned int i = Temp; i != mSize; ++i) {
					mThreadPool.emplace_back(std::thread(&ThreadPool::UpdatePool, this));
					mThreadPool.back().detach();
				}
			}
			else {
				mSize = n;
				mThreadPool.resize(n);
			}
		}

		unsigned ThreadPool::GetThreadCount() {
			return mSize;
		}

		void ThreadPool::UpdatePool() {
			for (;;) {
				std::unique_lock<std::mutex> Lock(mTaskMutex);

				mThreadCond.wait(Lock, [this] { return !mTaskQueue.empty(); });

				if (mTaskQueue.size() >= 1) {
					(*mTaskQueue.front()).Run();
					mTaskQueue.pop();
				}
			}
		}
	}
}