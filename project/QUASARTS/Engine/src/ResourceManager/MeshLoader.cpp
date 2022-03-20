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
namespace Engine
{

	MeshLoaderFactory::MeshLoaderFactory()
	{
		add_loader<ObjLoader>();
		//later add other loader like fbx/gltf...........
	}

	void MeshLoaderFactory::load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>&)
	{
	}

	void MeshLoader::load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>&)
	{
	}

}