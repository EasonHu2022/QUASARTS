#include "Material.h"
namespace Engine
{
	Material::Material(std::string _vShaderPath, std::string _fShaderPath, std::string _gShaderPath, std::string _texture)
	{
		vShaderPath = _vShaderPath;
		fShaderPath = _fShaderPath;
		gShaderPath = _gShaderPath;
		texture = _texture;
		char* gPath;
		if (_gShaderPath == "")
			gPath = NULL;
		else
			gPath = _gShaderPath.data();
		shader = new Shader(vShaderPath.data(), fShaderPath.data(), gPath);
	}


	Material::~Material()
	{
	}

};