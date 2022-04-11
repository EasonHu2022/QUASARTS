#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API TransformComponent {
        float x;
        float y;
        float z;
        float rotation;
        float scale;
    };
}