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
namespace Engine
{

	MeshLoaderFactory::MeshLoaderFactory()
	{
		add_loader<ObjLoader>();
		//later add other loader like fbx/gltf...........
	}

	void MeshLoaderFactory::load(const std::string& obj, std::unordered_map<std::string, std::shared_ptr<Mesh>>& meshes)
	{
		auto extension = getExtension(obj);
		auto loader = loaders.find(extension);
		if (loader != loaders.end())
		{
			loader->second->load(obj, extension, meshes);
		}
		else
		{
			QERROR( " Unknown file extension {0}", extension);
		}
	}

	void MeshLoader::load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>& meshes)
	{
		//Application::getModelLoaderFactory()->load(obj, meshes,);
	}

}