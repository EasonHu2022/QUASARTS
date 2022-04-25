#pragma once
#include "Core/IResource.h"
#include "Core/Core.h"
#include <string>

namespace Engine
{
	class QS_API FileResource : public IResource
	{
	public:
		std::string textContent;

		void reload() override {};
	};	
}
