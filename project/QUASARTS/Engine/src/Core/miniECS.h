#pragma once
#include "Core/Core.h"
#include "ResourceManager/AttributeVector.h"
#include <vector>
namespace Engine
{
	class QS_API miniECS
	{
	public:
		miniECS();
		~miniECS();
		
		void add_entity(AttributeVector* attrib);

		void remove_entity(AttributeVector* attrib);

		void set_index(int* ind);

		std::vector<AttributeVector*> entities;

		int index;

	};


};