#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "Core/Core.h"
#include "Core/Mesh.h"
#include <vector>
#include <string>

namespace Engine
{

	class QS_API ModelResource
	{
	public:
		ModelResource();
		~ModelResource();
		std::unordered_map<std::string, std::shared_ptr<Engine::Mesh>> meshes;
		std::string name;
	};


}



