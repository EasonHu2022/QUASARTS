#pragma once

// Library includes:

#include <string>

// Local includes:
#include "ECS/ECS-Common.h"
#include "glm/glm.hpp"

namespace Engine {
    enum class LightType
    {
        def = 0,
        spot,
        parallel,
        point,
    }; 

    struct QS_API LightComponent {
        LightComponent() {
            
        };
        LightType type = LightType::def;
        glm::vec3 ambient = {0.2f,0.2f,0.2f};
        glm::vec3 diffuse = { 0.2f,0.2f,0.2f };
        glm::vec3 specular = { 0.2f,0.2f,0.2f };
        
        
    };

    struct Lightinfo// aligned with std140
    {
        Lightinfo() {}
        /*count of all of the lights in the scene, max = 10*/
        int countLight = 0;
        LightComponent lights[50];

    };
}