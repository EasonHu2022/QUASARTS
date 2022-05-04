#include "Material.h"

namespace Engine
{
	Material::Material(std::string _vShaderPath, std::string _fShaderPath, std::string _gShaderPath, std::string _texture)
	{
		vShaderPath = _vShaderPath;
		fShaderPath = _fShaderPath;
		gShaderPath = _gShaderPath;
		texturePath = _texture;
		char* gPath;
		if (_gShaderPath == "")
			gPath = NULL;
		else
			gPath = _gShaderPath.data();
		shader = new Shader(vShaderPath.data(), fShaderPath.data(), gPath);

		if (_texture != "")
			texture = new Texture2D(_texture);
		else
			texture = NULL;
	}


	Material::~Material()
	{
	}

};