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

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, LightComponent &light) {
        int light_type;
        inStream >> light_type;
        if (light_type == 1) { light.type = LightType::spot; }
        else if (light_type == 2) { light.type = LightType::parallel; }
        else if (light_type == 3) { light.type = LightType::point; }
        else { light.type = LightType::def; }
        
        inStream
        >> light.ambient[0] >> light.ambient[1] >> light.ambient[2]
        >> light.diffuse[0] >> light.diffuse[1] >> light.diffuse[2]
        >> light.specular[0] >> light.specular[1] >> light.specular[2];

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const LightComponent &light) {
        if (light.type == LightType::spot) { outStream << 1; }
        else if (light.type == LightType::parallel) { outStream << 2; }
        else if (light.type == LightType::point) { outStream << 3; }
        else { outStream << 0; }

        outStream
        << light.ambient[0] << light.ambient[1] << light.ambient[2]
        << light.diffuse[0] << light.diffuse[1] << light.diffuse[2]
        << light.specular[0] << light.specular[1] << light.specular[2];
        
        return outStream;
    }
}