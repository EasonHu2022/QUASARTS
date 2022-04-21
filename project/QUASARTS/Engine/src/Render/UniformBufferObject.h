#pragma once
#include <stdint.h>
namespace Engine
{
	enum class UniformBlockIndex
	{
		invalid = -1,
		matrices = 0,
		light = 1,
	};
	class UniformBufferObject
	{
	private:
		UniformBufferObject() {};
		UniformBlockIndex slot = UniformBlockIndex::invalid;
		uint32_t handle = 0;
		size_t size = 0;
	public:
		UniformBufferObject(UniformBlockIndex slot, size_t size);
		~UniformBufferObject();
		void init();
		void set_data(uint32_t offset, uint32_t size, const void* data);


	};
};