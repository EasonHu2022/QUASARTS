#pragma once
#include "Core/Core.h"
#include <string>
#include "glfw3.h"
#include "glad.h"
#include "glm/glm.hpp"
#include <vector>

namespace Engine
{
	class QS_API CubeMap 
	{
	public:
		CubeMap(std::vector<std::string> paths);
		~CubeMap();
		unsigned int cubeMapHandle;
		void use(GLenum texture);
		//texture path


	};
};