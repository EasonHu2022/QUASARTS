#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "glm/glm.hpp"

namespace Engine {
    struct QS_API CameraComponent {
        float fov = 45.0f;
        float ratio = 1.0f;
        float nearClip = 0.1f;
        float farClip = 100.0f;
    };
}