#include "miniECS.h"
namespace Engine {
	miniECS::miniECS()
	{
	}

	miniECS::~miniECS()
	{
	}

	void miniECS::add_entity(AttributeVector* attrib)
	{
		entities.push_back(attrib);
	}

	void miniECS::remove_entity(AttributeVector* attrib)
	{
	}

};