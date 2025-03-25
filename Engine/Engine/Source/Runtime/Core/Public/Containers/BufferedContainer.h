#pragma once
#include "CoreTypes.h"

template<class TContainer, class TItem>
class TBufferedContainer
{
public:
	TContainer& GetContainer() { return Buffers[(BufferIndex + 1) % 2]; }
	const TContainer& GetContainer() const { return Buffers[(BufferIndex + 1) % 2]; }

	void AddItem(TItem&& Item)
	{
		unique_lock<mutex> Lock(Mutex);
		Buffers[BufferIndex].emplace(std::forward<TItem&&>(Item));
	}

	void AddItem(const TItem& Item)
	{
		unique_lock<mutex> Lock(Mutex);
		Buffers[BufferIndex].emplace(Item);
	}

	void SwapBuffers()
	{
		unique_lock<mutex> Lock(Mutex);
		BufferIndex ^= 1;
	}

	bool IsEmpty() const
	{
		unique_lock<mutex> Lock(Mutex);
		return Buffers[BufferIndex].empty();
	}

private:
	mutable mutex Mutex;
	array<TContainer, 2> Buffers;
	int32 BufferIndex = 0;
};