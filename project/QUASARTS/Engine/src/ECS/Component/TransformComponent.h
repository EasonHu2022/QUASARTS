#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API TransformComponent {
        float position;
        float rotation;
        float scale;
    };
}