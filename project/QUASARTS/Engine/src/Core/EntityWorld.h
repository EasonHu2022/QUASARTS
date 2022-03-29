#pragma once
#include "Core/Core.h"
#include "ResourceManager/ModelResource.h"
#include <vector>
namespace Engine
{
	class QS_API EntityWorld
	{
	public:
		EntityWorld();
		~EntityWorld();
		
		void add_entity(ModelResource* model);

		void remove_entity(ModelResource* model);

		std::vector<ModelResource*> models;

	};


};