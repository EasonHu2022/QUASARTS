#pragma once
#include "IResource.h"
#include <string>

namespace Engine
{
	class FileResource : public IResource
	{
	public:
		std::string textContent;

		void reload() override {};
	};	
}
