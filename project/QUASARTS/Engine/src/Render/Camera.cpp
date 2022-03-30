#include "Camera.h"

namespace Engine
{
	Camera::Camera(float fov, float, float, float aspect)
	{
	}
	void Camera::updateProjectionMatrix()
	{
		projMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}


};
