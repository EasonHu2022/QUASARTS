#pragma once
#include <cstdint>
#include "Core/Core.h"
#include "Core/Mesh.h"
#include <vector>
#include "glm/glm.hpp"

namespace Engine
{

	class QS_API AttributeVector
	{
	public:
		AttributeVector();
		~AttributeVector();
		std::vector<glm::vec3> attributes;
	};


}


