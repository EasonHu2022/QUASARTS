#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

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

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, TransformComponent &transform) {
        inStream
        >> transform.position[0] >> transform.position[1] >> transform.position[2]
        >> transform.rotation[0] >> transform.rotation[1] >> transform.rotation[2]
        >> transform.scale[0] >> transform.scale[1] >> transform.scale[2];

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const TransformComponent &transform) {
        outStream
        << transform.position[0] << " " << transform.position[1] << " " << transform.position[2]
        << " " << transform.rotation[0] << " " << transform.rotation[1] << " " << transform.rotation[2]
        << " " << transform.scale[0] << " " << transform.scale[1] << " " << transform.scale[2];
        
        return outStream;
    }

}