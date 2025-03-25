#pragma once
#include "CoreMinimal.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

namespace ENamedThreads
{
	enum Type : int32
	{
		GameThread,

		AnyThread,
	};
}

using FTaskQueue = TBufferedContainer<queue<function<void()>>, function<void()>>;
using FTask = function<void()>;

class CORE_API FTaskManager
{
public:
	FTaskManager();
	~FTaskManager();
	void Tick();

	static FTaskManager* Get(const bool bDestroy = false);

	template<ENamedThreads::Type Thread>
	void AddTask(FTask& Task)
	{
		if constexpr (Thread == ENamedThreads::GameThread)
		{
			TaskQueue.AddItem(Task);
		}
		else if constexpr (Thread == ENamedThreads::AnyThread)
		{
			boost::asio::post(*ThreadPool, Task);
		}
		else
		{
			_ASSERT(false);
		}
	}

protected:
	unique_ptr<boost::asio::thread_pool> ThreadPool;

	FTaskQueue TaskQueue;
};

template<ENamedThreads::Type Thread>
void AsyncTask(FTask Task)
{
	FTaskManager::Get()->AddTask<Thread>(Task);
}