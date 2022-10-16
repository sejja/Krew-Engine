#ifndef _THREAD_POOL__H_
#define _THREAD_POOL__H_

namespace Engine {
	namespace Threading {
		class ENGINE_API ThreadPool {
		public:
			ThreadPool(const ThreadPool&) = delete;
			ThreadPool& operator=(const ThreadPool&) = delete;
			ThreadPool(unsigned n = std::thread::hardware_concurrency());
			~ThreadPool();

			template <typename Fn, typename... ARGS >
			auto AddTask(Fn&& fn, ARGS&&... args) -> std::future<decltype(fn(args...))> {
				typedef decltype(fn(args...)) ReturnType;

				std::packaged_task<ReturnType()> NewTask(std::move(std::bind(fn, args...)));
				std::unique_lock<std::mutex> Lock(mTaskMutex);
				std::future<ReturnType> Future = NewTask.get_future();

				mTaskQueue.emplace(std::make_unique<Task<ReturnType> >(std::move(NewTask)));
				mThreadCond.notify_one();

				return Future;
			}

			void Resize(unsigned num);
			unsigned GetThreadCount();

		private:
			void UpdatePool();

			class ITask {
			public:
				virtual ~ITask() {}
				virtual void Run() = 0;
			};

			template <typename ReturnType>
			class Task : public ITask {
			public:
				Task(std::packaged_task<ReturnType()> fn) : Fn(std::move(fn)) {}
				
				void Run() { 
					Fn(); 
				}
			private:
				
				std::packaged_task<ReturnType()> Fn;
			};

			std::vector<std::thread> mThreadPool;
			std::queue<std::unique_ptr<ITask>> mTaskQueue;

			unsigned mSize;
			std::condition_variable mThreadCond;
			std::mutex mTaskMutex;
		};
	}
}

#endif