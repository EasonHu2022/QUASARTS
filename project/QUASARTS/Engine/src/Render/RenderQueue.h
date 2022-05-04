#pragma once
#include <stdint.h>
#include <queue>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Shader.h"
#include "Texture2D.h"
class pack
{
public:
	pack(){};
	uint32_t _VAO = -1;
	uint32_t size = 0;
	Engine::Shader *shader_program = NULL;
	Engine::Texture2D* texture2d = NULL;
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 normalM = glm::mat4(1.0f);

	inline void set_model(glm::vec3 offset, glm::vec3 rotation, glm::vec3 scale)
	{
		model = glm::translate(model, offset);
		//x
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
		//y
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		//z
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

		model = glm::scale(model, scale);
		normalM = glm::transpose(glm::inverse(model));
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


