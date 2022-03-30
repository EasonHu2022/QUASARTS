#include "EntityWorld.h"
namespace Engine {
	EntityWorld::EntityWorld()
	{
	}

	EntityWorld::~EntityWorld()
	{
	}

	void EntityWorld::add_entity(ModelResource* model)
	{
		models.push_back(model);
	}

	void EntityWorld::remove_entity(ModelResource* model)
	{
	}

	void EntityWorld::render()
	{
		for (auto m : models)
		{
			m->render();
		}
	}

};