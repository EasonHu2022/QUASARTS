#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include "Core/Core.h"
#include "Core/Mesh.h"
#include <vector>
#include <string>
#include <memory>

namespace Engine
{

	class QS_API ModelResource : public IResource
	{
	public:
		ModelResource();
		~ModelResource();
		std::unordered_map<std::string, std::shared_ptr<Engine::Mesh>> meshes;
		std::string name;
		void render();

		void reload() override {}
	};


}



