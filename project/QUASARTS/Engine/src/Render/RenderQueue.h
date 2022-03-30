#pragma once
#include <stdint.h>
#include <queue>
class RenderQueue
{
private:
	std::queue<uint32_t> queue;
public:
	/*later will have something like set_shader()*/
	RenderQueue();
	~RenderQueue();
	void push(uint32_t handle);
	void pop();
	uint32_t get();
	unsigned int get_size();

	
};


