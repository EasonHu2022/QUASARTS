#pragma once

// Library includes:

#include <string>

// Local includes:
#include "ECS/ECS-Common.h"
#include "Render/Material.h"

namespace Engine {
    struct QS_API MaterialComponent {
       
        std::string path = "";
        //for created in memory
        Material* material = NULL;
    };
}