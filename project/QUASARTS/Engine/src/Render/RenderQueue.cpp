#include "RenderQueue.h"

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
}

void RenderQueue::push(uint32_t handle)
{
	queue.push(handle);
}

void RenderQueue::pop()
{
	queue.pop();
}

uint32_t RenderQueue::get()
{
	return queue.front();
}

unsigned int RenderQueue::get_size()
{
	return queue.size();
}
