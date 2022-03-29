#include "miniECS.h"
namespace Engine {
	miniECS::miniECS()
	{
		index = -1;
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

	void miniECS::set_index(int* ind)
	{
		index = *ind;
	}

};