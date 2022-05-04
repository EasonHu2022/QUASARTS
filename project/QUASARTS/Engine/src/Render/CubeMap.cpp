#include "CubeMap.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/TextureResource.h"
namespace Engine
{
	//the order is right, left, top, bottom,front, back, 
	CubeMap::CubeMap(std::vector<std::string> paths)
	{
		glGenTextures(1,&cubeMapHandle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapHandle);
		for (int i = 0;i < paths.size();i++)
		{
			auto p = paths[i];
			size_t handle;
			bool ret = ResourceManager::Instance()->load_resource(p, &handle);
			if (!ret)
				return;
			auto data = ResourceManager::Instance()->get_resource<TextureResource>(handle)->data;
			auto width = ResourceManager::Instance()->get_resource<TextureResource>(handle)->width;
			auto height = ResourceManager::Instance()->get_resource<TextureResource>(handle)->height;
			auto nrChannels = ResourceManager::Instance()->get_resource<TextureResource>(handle)->nrChannels;
			if (nrChannels == 3)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
			if (nrChannels == 4)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
				);
		
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	CubeMap::~CubeMap()
	{
	}

	void CubeMap::use(GLenum texture)
	{
		glActiveTexture(texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapHandle);
	}
};