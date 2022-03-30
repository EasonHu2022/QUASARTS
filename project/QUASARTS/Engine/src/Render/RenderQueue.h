#pragma once
#include <stdint.h>
#include <queue>
struct pack
{
public:
	pack(uint32_t vao, uint32_t size) :_VAO(vao), size(size) {};
	uint32_t _VAO;
	uint32_t size;
};
class RenderQueue
{
private:
	std::queue<pack> queue;
public:
	/*later will have something like set_shader()*/
	RenderQueue();
	~RenderQueue();
	void push(pack handle);
	void pop();
	pack get();
	unsigned int get_size();

	
};


