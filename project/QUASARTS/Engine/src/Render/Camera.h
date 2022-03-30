#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Engine
{
	class Camera
	{
	public:
		Camera(float fov, float near, float far, float aspect);

	public:
		//temp use be public and positon be attri without ECS
		glm::mat4 projMatrix = glm::mat4(1.f);
		glm::vec3 position = glm::vec3(0.f,0.f,1.f);


		float aspectRatio = 0.0f;
		float scale = 100.0f;
		float zoom = 1.0f;
		
		void updateProjectionMatrix();
		float fov = 0.0f;
		float nearClip = 0.0f;
		float farClip = 0.0f;

	};
};