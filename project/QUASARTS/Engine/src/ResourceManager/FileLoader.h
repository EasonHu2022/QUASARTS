#pragma once
#include "Core/Core.h"
#include <string>
#include "IResource.h"
#include "FileResource.h"
namespace Engine
{
	


	class QS_API FileLoader 
	{
	public:
		//static constexpr char* EXTENSIONS[] = { "VertShader" ,"FragShader", "GeoShader","Lua"};
		size_t load(const std::string& obj);

	};

}



