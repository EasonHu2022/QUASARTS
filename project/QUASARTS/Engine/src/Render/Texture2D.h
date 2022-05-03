#pragma once
#include "Core/Core.h"
#include <string>
#include "glm/glm.hpp"
#include "ITexture.h"
namespace Engine
{
	class QS_API Texture2D : public ITexture
	{
	public:
		Texture2D(std::string path,int width = 0, int height = 0);
		~Texture2D();
		void set_up() override;
		void use(GLenum texture) override;
		//texture path


	};
};