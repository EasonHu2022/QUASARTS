#pragma once
#include "Core/IResource.h"
#include <string>

namespace Engine
{
	class FileResource : public IResource
	{
	public:
		std::string textContent;
		std::string fileShortname;
		std::string extension;
		void reload() override {};
	};	
}
