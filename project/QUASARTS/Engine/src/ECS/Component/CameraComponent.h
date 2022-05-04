#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "glm/glm.hpp"

namespace Engine {
    struct QS_API CameraComponent {
        float fov = 60.0f;
        float ratio = 800.0f/600.0f;
        float nearClip = 0.1f;
        float farClip = 100.0f;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, CameraComponent &camera) {
        inStream >> camera.fov >> camera.ratio >> camera.nearClip >> camera.farClip;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const CameraComponent &camera) {
        outStream << camera.fov << " " << camera.ratio << " " << camera.nearClip << " " << camera.farClip;
        
        return outStream;
    }
}