#pragma once
#include "Core/Core.h"
#include "Core/IResource.h"
#include <Render/Shader.h>
#include <string>
#include "glm/glm.hpp"
#include "Texture2D.h"
namespace Engine
{
	class QS_API Material : public IResource
	{
	public:
		Material(std::string vShaderPath, std::string fShaderPath, std::string gShaderPath = "", std::string texture = "");
		~Material();
		std::string name = "default";
		std::string vShaderPath = "";
		std::string fShaderPath = "";
		std::string gShaderPath = "";
		Shader* shader = NULL;
		
		glm::vec3 ambient = { 0.2f,0.2f,0.2f };
		glm::vec3 diffuse = { 0.2f,0.2f,0.2f };
		glm::vec3 specular = { 0.2f,0.2f,0.2f };
		glm::vec3 emission = { 0.0f,0.0f,0.0f };
		float emissiveRange = 0.0f;
		float shininess = 32.0f;
		Texture2D* texture;
		std::string texturePath = "";
		void reload() override {}
		//texture path


	};
};