#pragma once
#include "Core/Core.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/TextureResource.h"
#include "glfw3.h"
#include "glad.h"
#include <string>
#include "glm/glm.hpp"
namespace Engine
{
	class QS_API ITexture
	{
	public:
		ITexture(std::string path)
		{
			size_t handle;
			bool ret = ResourceManager::Instance()->load_resource(path,&handle);
			if (!ret)
				return;
			data = ResourceManager::Instance()->get_resource<TextureResource>(handle)->data;
			width = ResourceManager::Instance()->get_resource<TextureResource>(handle)->width;
			height = ResourceManager::Instance()->get_resource<TextureResource>(handle)->height;
			nrChannels = ResourceManager::Instance()->get_resource<TextureResource>(handle)->nrChannels;
			
		};
		~ITexture() {};
		virtual void use(GLenum texture) = 0;
		virtual void set_up() = 0;
	public:
		int width;
		int height;
		int nrChannels;
		unsigned char* data;
		unsigned int textureObject;

		//texture path


	};
};