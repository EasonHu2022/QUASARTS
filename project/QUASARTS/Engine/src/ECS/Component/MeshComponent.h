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

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, MeshComponent &mesh) {
        inStream >> mesh.resId >> mesh.path;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const MeshComponent &mesh) {
        outStream << mesh.resId << " " << mesh.path;
        
        return outStream;
    }
}