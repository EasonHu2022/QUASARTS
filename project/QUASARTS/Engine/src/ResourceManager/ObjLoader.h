#pragma once
#include "Core/Core.h"
#include "ResourceManager/MeshLoader.h"
namespace Engine
{

	class ObjLoader : public MeshLoader
	{
	public:
		static constexpr char* EXTENSIONS[] = { "obj" };
		void load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>&) override;

	};

}



