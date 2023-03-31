#ifndef TH
#define TH

// C++17

#include<mutex>
#include<vector>
#include<thread>
#include<condition_variable>
#include<atomic>
#include<future>
#include<queue>
#include"noCopy.h"

namespace TianHui
{
	const int MAX_THREAD_SIZE = 1024;	// 默认线程最大数量
	const int MAX_TASK_SIZE = 1024;		// 默认任务的最大数量


	template<typename T>
	class mLambda;

	template<typename Re, typename ...Args>
	class mLambda<Re(Args...)>final : noCopy
	{
	public:
		template<typename L>
		explicit mLambda(const L& lam) :
			fatherPtr_{ new son(lam) }
		{
			static_assert(std::is_invocable<L>::value, "不是lambda表达式");
		}

		mLambda(const mLambda& p) :
			fatherPtr_(p.fatherPtr_)
		{
		}

		Re operator()(Args&&...args)
		{
			return fatherPtr_->operator()(std::forward<Args>(args)...);
		}

	private:

		struct father
		{
			father() = default;

			virtual ~father() = default;

			virtual inline Re operator()(Args&&...args) const = 0;

		};

		template<typename F>
		struct son final :father
		{

			son(const F& f) :
				lambda_(f) {}

			~son() = default;

			virtual inline Re operator()(Args&&...args) const override
			{
				return lambda_(std::forward<Args>(args)...);
			}


			F lambda_;
		};

		std::shared_ptr<father>fatherPtr_;

	};




	enum class Mode
	{
		FIXED,
		CATCH
	};

	enum class threadMode
	{
		Join,
		Detch
	};

	class ThreadPool final :noCopy
	{

	public:


		explicit ThreadPool() :
			maxThreadSize_(MAX_THREAD_SIZE),
			maxTaskSize_(MAX_TASK_SIZE),
			taskSize_(0),
			threadSize_(0),
			isEnd_(false),
			mode_(Mode::FIXED)
		{}


		~ThreadPool()
		{
			isEnd_ = true;

			{
				std::unique_lock<std::mutex>lock(mutexPool_);
				taskSize_ = MAX_TASK_SIZE;
				notEmpty_.notify_all();

				endPool_.wait(lock, [this]()->bool {return threadSize_ <= 0; });

			}

			threadPool_.clear();

		};


		void setThreadSize(int size)
		{
			maxThreadSize_ = size;
		}


		void setTaskSize(int size)
		{
			maxTaskSize_ = size;
		}


		void setPoolMode(Mode mode)
		{
			mode_ = mode;
		}

		template<typename _Ty, typename ..._list>
		inline auto submit(_Ty(&& _callback), _list && ...t)->std::future< decltype(_callback(t...)) >;


		void start(int size = 4,const threadMode&m=threadMode::Detch)
		{
			for (int i = 0; i < size; ++i, ++threadSize_)
			{
				threadPool_.push_back(std::make_shared<Thread>(mLambda<void()>([=]() {this->taskFunction(); })));
			}

			for (int i = 0; i < size; ++i)
				threadPool_[i]->star(m);

		}

	private:


		class Thread
		{
		public:

			Thread(mLambda<void()>&& func) :
				func_{ func }
			{}

			~Thread() {};

			void star(const threadMode&m)
			{
				std::thread t(func_);

				switch (m)
				{
				case threadMode::Join:
					t.join();
					break;

				default:
					t.detach();
					break;
				}
				

			}


		private:
			mLambda<void()>func_;

		};


	private:

		bool isEnding()
		{
			return isEnd_;
		}


		void taskFunction()
		{

			for (; !isEnding();)
			{

				std::cout << std::this_thread::get_id() << " :" << "Test获取任务" << std::endl;
				if (isEnding())
					break;

				std::unique_lock<std::mutex>lock(mutexPool_);
				notEmpty_.wait(lock, [this]()->bool {return taskSize_ > 0; });
				if (isEnding() && taskQueue_.size() <= 0)
					break;

				--taskSize_;

				std::cout << "\\\\\\\\\\\\\\\\\\" << std::this_thread::get_id() << " :" << "获取任务 Succeed!\\\\\\\\\\\\\n\r" << std::endl;

				auto& temp = taskQueue_.front();
				temp();

				notFull_.notify_all();

				taskQueue_.pop();

				if (taskSize_ > 0)
					notEmpty_.notify_all();

			}

			--threadSize_;

			endPool_.notify_all();
			std::cout << std::this_thread::get_id() << " :" << "thread exit!" << std::endl;
			return;
		}

	private:

		std::mutex mutexPool_;

		int maxThreadSize_;

		int maxTaskSize_;

		std::atomic_int taskSize_;

		std::atomic_int threadSize_;

		Mode mode_;

		std::queue<mLambda<void()>>taskQueue_;

		std::vector<std::shared_ptr<Thread>> threadPool_;

		std::condition_variable notEmpty_;

		std::condition_variable notFull_;

		std::condition_variable endPool_;

		std::atomic_bool isEnd_;
	};




	template<typename _Ty, typename ..._list>
	inline auto ThreadPool::submit(_Ty(&& _callback), _list && ...t)->std::future< decltype(_callback(t...)) >
	{
		using Rtype = decltype(_callback(t...));
		//auto time = std::chrono::high_resolution_clock::now();

		auto task = std::shared_ptr<std::packaged_task< Rtype()>>{ new std::packaged_task< Rtype()>([_callback, t...]()->Rtype {return _callback(t...); }) };

		{
			std::unique_lock<std::mutex>lock(mutexPool_);
			if (!notFull_.wait_for(lock, std::chrono::seconds(1), [this]()->bool {return taskSize_ < maxTaskSize_; }))
			{
				std::cerr << "任务队列已满,任务提交失败！" << std::endl;
				throw "任务队列已满,任务提交失败！";
			}

			taskQueue_.emplace(mLambda<void()>(
				[task]()->void
				{
					(*task)();
				}));
		}

		++taskSize_;
		notEmpty_.notify_all();

		return task->get_future();

	}





}


using threadpool = TianHui::ThreadPool;














#endif // !TH
