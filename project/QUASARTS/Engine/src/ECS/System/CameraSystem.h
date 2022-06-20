#pragma once

// Player controls:
#define PLAYER_CTRL_MOUSE_MODE GLFW_KEY_F11
#define PLAYER_CTRL_FORWARD GLFW_MOUSE_BUTTON_RIGHT
#define PLAYER_CTRL_BACK GLFW_KEY_V
#define PLAYER_CTRL_LEFT GLFW_KEY_X
#define PLAYER_CTRL_RIGHT GLFW_KEY_B

#include "Core/Core.h"
#include "Core/Application.h"
#include "ECS/ECSManager.h"
#include "ECS/ECS-Common.h"
#include "Time/TimeModule.h"
#include "ECS/Entity-Stress-Test.h"

#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
    class QS_API CameraSystem : public System
    {
        private:
        // Singleton:
        static CameraSystem* instance;
        CameraSystem();

        public:
        static CameraSystem* Instance();
        ~CameraSystem();

        void init() {}
        int start() { return 0; }
        void update();
        int stop() { return 0; }
        void release() {}

        // Set up all the data required for the component to function:
        void initialize_components() override {}
    };
}