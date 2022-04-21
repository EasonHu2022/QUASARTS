#pragma once
#include "Core/Core.h"
#include "ResourceManager/IResource.h"
#include <Render/Shader.h>
#include <string>
#include "glm/glm.hpp"
namespace Engine
{
	class Material : public IResource
	{
	public:
		Material(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath = "", std::string texture = "");
		~Material();
		std::string vShaderPath = "";
		std::string fShaderPath = "";
		std::string gShaderPath = "";
		Shader* shader = NULL;
		std::string texture = "";
		glm::vec3 ambient = { 0.2f,0.2f,0.2f };
		glm::vec3 diffuse = { 0.2f,0.2f,0.2f };
		glm::vec3 specular = { 0.2f,0.2f,0.2f };
		float shininess = 0.4f;

		void reload() override {};
		//texture path


	};
};