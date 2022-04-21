#pragma once
#include <stdint.h>
#include <queue>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Shader.h"
class pack
{
public:
	pack(){};
	uint32_t _VAO = -1;
	uint32_t size = 0;
	Engine::Shader *shader_program = NULL;
	uint32_t tex = -1;
	glm::mat4 model = glm::mat4(1.0f);
	

	inline void set_model(glm::vec3 offset, glm::vec3 rotation, glm::vec3 scale)
	{
		model = glm::scale(model, scale);
		model = glm::translate(model, offset);
		//x
		model = glm::rotate(model, rotation.x, glm::vec3(1,0,0));
		//y
		model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
		//z
		model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));		
	}

	
};
class RenderQueue
{
private:
	std::queue<pack*> queue;
public:
	/*later will have something like set_shader()*/
	RenderQueue();
	~RenderQueue();
	void push(pack* handle);
	void pop();
	pack* get();
	unsigned int get_size();

	
};


