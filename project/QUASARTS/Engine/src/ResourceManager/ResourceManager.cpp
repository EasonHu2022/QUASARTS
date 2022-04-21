#include "ResourceManager.h"
namespace Engine
{
	ResourceManager* ResourceManager::instance = NULL;
	ResourceManager* ResourceManager::Instance()
	{
		if (NULL == instance)
		{
			instance = new ResourceManager();
		}
		return instance;
	}
};