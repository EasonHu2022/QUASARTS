#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "glm/glm.hpp"

namespace Engine {
    struct QS_API TransformComponent {
        float x;
        float y;
        float z;
        glm::vec3 position = {0.0f,0.0f,0.0f};
        //x->y->z
        glm::vec3 rotation = {0.0f,0.0f,0.0f};
        glm::vec3 scale = {1.0f,1.0f,1.0f};

    };
}