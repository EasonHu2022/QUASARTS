#pragma once
#include "Core/IModule.h"
#include "Core/Core.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "Core/Mesh.h"
#include "MeshLoader.h"
#include "ObjLoader.h"
#include "Core/QUtil.h"
#include "Logger//LogModule.h"
#include "Core/Application.h"
#include "ResourceManager/ResourceManager.h"
namespace Engine
{

	MeshLoaderFactory::MeshLoaderFactory()
	{
		add_loader<ObjLoader>();
		//later add other loader like fbx/gltf...........
	}

	size_t MeshLoaderFactory::load(const std::string& obj)
	{
		ResourceManager* res = ResourceManager::Instance();

		std::hash<std::string> h;
		size_t handle = h(obj);

		auto exist = res->get_resource<ModelResource>(handle);
		if (exist.get() == NULL)
		{
			auto modelResource = std::make_shared<ModelResource>();
			modelResource.get()->path = obj;
			modelResource.get()->refs = 1;
			auto extension = getExtension(obj);
			auto loader = loaders.find(extension);
			if (loader != loaders.end())
			{
				loader->second->load(obj, extension, modelResource->meshes);
			}
			else
			{
				QERROR(" Unknown file extension {0}", extension);
			}


			return res->add_resource<ModelResource>(modelResource);
		}

		exist.get()->refs++;

		return handle;
	}

	void MeshLoader::load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>& meshes)
	{
		//Application::getModelLoaderFactory()->load(obj, meshes,);
	}

}