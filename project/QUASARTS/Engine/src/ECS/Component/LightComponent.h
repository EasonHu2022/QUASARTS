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
       
        glm::vec3 ambient = {0.2f,0.2f,0.2f};
        glm::vec3 diffuse = { 0.2f,0.2f,0.2f };
        glm::vec3 specular = { 0.2f,0.2f,0.2f };   
        int type = 0;
    };

    struct alignas(16) lightBuffer
    {
        glm::vec3 ambient = { 0.2f,0.2f,0.2f };
        glm::vec3 diffuse = { 0.2f,0.2f,0.2f };
        glm::vec3 specular = { 0.2f,0.2f,0.2f };
        glm::vec3 positon = { 0.0f,0.0f,0.0f };
        float type = 0;

    };

    struct alignas(16) Lightinfo// aligned with std140
    {
        /*count of all of the lights in the scene, max = 10*/
       
        lightBuffer lights[10];
        float countLight = 0;
    };
}