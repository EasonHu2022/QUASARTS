#pragma once
#include "Core/Core.h"
#include "MeshLoader.h"
namespace Engine
{

	class ObjLoader : MeshLoader
	{
	public:
		static constexpr char* EXTENSIONS[] = { "obj" };
		void load(const std::string& obj, const std::string& extension, std::unordered_map<std::string, std::shared_ptr<Mesh>>&) override;

	};

}



