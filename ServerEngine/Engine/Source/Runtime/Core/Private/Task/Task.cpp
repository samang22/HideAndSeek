#include "Task/Task.h"

FTaskManager::FTaskManager()
{
	const uint32 ThreadCount = std::thread::hardware_concurrency() - 1u;
	E_LOG(Log, TEXT("FTaskManager Create Thread pool {}"), ThreadCount);
	ThreadPool = make_unique<boost::asio::thread_pool>(ThreadCount);
}

FTaskManager::~FTaskManager()
{
	ThreadPool->join();
	ThreadPool.reset();
}

void FTaskManager::Tick()
{
	if (TaskQueue.IsEmpty()) { return; }

	TaskQueue.SwapBuffers();

	queue<FTask>& Tasks = TaskQueue.GetContainer();
	FTask Task;
	while (!Tasks.empty())
	{
		Task = move(Tasks.front());
		Tasks.pop();

		Task();
	}
}

FTaskManager* FTaskManager::Get(const bool bDestroy)
{
	static unique_ptr<FTaskManager> Instance = make_unique<FTaskManager>();
	if (bDestroy)
	{
		Instance.reset();
	}
	return Instance.get();
}
