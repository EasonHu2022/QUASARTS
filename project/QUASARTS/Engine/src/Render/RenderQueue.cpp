#include "RenderQueue.h"

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
}

void RenderQueue::push(pack handle)
{
	queue.push(handle);
}

void RenderQueue::pop()
{
	queue.pop();
}

pack RenderQueue::get()
{
	return queue.front();
}

unsigned int RenderQueue::get_size()
{
	return queue.size();
}
