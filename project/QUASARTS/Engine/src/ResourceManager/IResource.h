#pragma once

#include <string>

namespace Engine
{
	class IResource
	{
	public: 
		std::string path;
		uint32_t refs = 0;
		virtual void reload() = 0;
	};	
}
