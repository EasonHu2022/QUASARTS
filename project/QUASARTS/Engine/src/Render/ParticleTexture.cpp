#pragma once

#include "ParticleTexture.h"
namespace Engine
{
	ParticleTexture::ParticleTexture(Texture2D* tex, int numrows, std::string name) {
		texture = tex;
		rows = numrows;
	}

};