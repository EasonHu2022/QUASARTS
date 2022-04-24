#include "UniformBufferObject.h"
#include "glad.h"
namespace Engine
{

	UniformBufferObject::UniformBufferObject(UniformBlockIndex _slot, size_t _size)
	{
		slot = _slot;
		size = _size;
		glGenBuffers(1, &handle);
	}
	UniformBufferObject::~UniformBufferObject()
	{
		glDeleteBuffers(1, &handle);
	}
	void UniformBufferObject::init()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)slot, handle);
	}
	void UniformBufferObject::set_data(uint32_t offset, uint32_t _size, const void* data)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, _size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

};
