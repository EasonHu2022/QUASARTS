#include "CameraSystem.h"

namespace Engine
{
    // Singleton:
    CameraSystem* CameraSystem::instance = nullptr;

    // Create the instance of the CombatSystem:
    CameraSystem* CameraSystem::Instance() {
        if (nullptr == instance)
            instance = new CameraSystem();
        return instance;
    }

    CameraSystem::CameraSystem() {
        // Set the Component mask for the camera:
        quasarts_component_mask mask;
        mask.mask = 0;
        mask.mask += (uint64_t)1 << COMPONENT_CAMERA;
        mask.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        add_component_mask(mask);
    }

    void CameraSystem::update()
    {
        // Get manager, declare Component pointers and get masks:
        ECSManager* active_manager = get_manager();
        unsigned int cameraID = active_manager->get_camera();
        TransformComponent *transform;

        // Check the time:
        auto deltaT = TimeModule::Instance()->get_frame_delta_time().sec();
        static float lastTime = 0.f;
        lastTime += deltaT;

        // Control configuration:
        float speed = 10.f;
        float thresholdTime = 0.2f;
        float mouseSensitivity = 10.f;

        // Get the Transform Component:
        transform = active_manager->get_component<TransformComponent>(cameraID, COMPONENT_TRANSFORM);

        // Player controls:
        GLFWwindow *app_window = Application::Instance->get_glfw_context();

        // Mouse rotation:
        double xPos, yPos;
        glfwGetCursorPos(app_window, &xPos, &yPos);
        static double lastX = xPos;
        static double lastY = yPos;

        // Mouse mode:
        if (GLFW_PRESS == glfwGetKey(app_window, PLAYER_CTRL_MOUSE_MODE))
        {
            if (lastTime >= thresholdTime)
                {
                if (glfwGetInputMode(app_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
                {
                    // Disable the cursor and reset last position so that the camera doesn't jump:
                    glfwSetInputMode(app_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    lastX = xPos;
                    lastY = yPos;
                }
                else if (glfwGetInputMode(app_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
                {
                    glfwSetInputMode(app_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
                lastTime = 0.f;
            }
        }

        if (glfwGetInputMode(app_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) { return; }

        // Mouse rotation:
		if (glfwGetInputMode(app_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			// Change in mouse position since last call:
			double dX = xPos - lastX;
			double dY = yPos - lastY;

			// Update pitch and clamp:
			transform->rotation.x -= dY * deltaT * mouseSensitivity;
			if (transform->rotation.x > 89.f) { transform->rotation.x = 89.f; }
			if (transform->rotation.x < -89.f) { transform->rotation.x = -89.f; }

			// Update yaw:
			transform->rotation.y += dX * deltaT * mouseSensitivity;
		}

		lastX = xPos;
		lastY = yPos;

        // Camera direction:
        glm::vec3 cameraDirection;
        cameraDirection.x = (float)(std::cos(glm::radians(transform->rotation.y))
                    * std::cos(glm::radians(transform->rotation.x)));

        cameraDirection.y = (float)(std::sin(glm::radians(transform->rotation.x)));

        cameraDirection.z = (float)(std::sin(glm::radians(transform->rotation.y))
                    * std::cos(glm::radians(transform->rotation.x)));

        // Camera front and up:
        glm::vec3 cameraFront = glm::normalize(cameraDirection);
        glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);

		// Movement keys. Start with forward and back (forward takes priority):
		if (GLFW_PRESS == glfwGetMouseButton(app_window, PLAYER_CTRL_FORWARD))
		{
			transform->position += (float)(speed * deltaT) * cameraFront;
		}
		else if (GLFW_PRESS == glfwGetKey(app_window, PLAYER_CTRL_BACK))
		{
			transform->position -= (float)(speed * deltaT) * cameraFront;
		}

		// Left and right. Left takes priority:
		if (GLFW_PRESS == glfwGetKey(app_window, PLAYER_CTRL_LEFT))
		{
			transform->position -= (float)(speed * deltaT)
				* glm::normalize(glm::cross(cameraFront, cameraUp));
		}
		else if (GLFW_PRESS == glfwGetKey(app_window, PLAYER_CTRL_RIGHT))
		{
			transform->position += (float)(speed * deltaT)
				* glm::normalize(glm::cross(cameraFront, cameraUp));
		}
    }
}