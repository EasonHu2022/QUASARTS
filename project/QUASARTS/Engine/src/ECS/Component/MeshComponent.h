#pragma once

// Library includes:
#include <string>

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API MeshComponent {
        float one;
        float two;
        float three;

        int resId = -1;
        std::string path;

    };
}