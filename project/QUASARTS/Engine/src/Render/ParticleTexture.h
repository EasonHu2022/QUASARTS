#pragma once
#include "Core/Core.h"
#include <string>
#include "glm/glm.hpp"
#include "Texture2D.h"

namespace Engine
{
	class QS_API ParticleTexture
	{
	public:
		Texture2D* texture;
		int rows;
		bool is_set;

		ParticleTexture(Texture2D* tex, int numrows, std::string name);
		ParticleTexture() {
			rows = 1;
			is_set = false;
		}
		~ParticleTexture() {}
		
	};

};