#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API ProjectileComponent {
        float speed = 1.0;
        float maxTime = 1.0;
        float currentTime = 0.0;
        unsigned int originID = TOO_MANY_ENTITIES;
        glm::vec3 direction;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, ProjectileComponent &projectile) {
        inStream >> projectile.speed >> projectile.maxTime >> projectile.currentTime
            >> projectile.originID >> projectile.direction.x >> projectile.direction.y
            >> projectile.direction.z;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const ProjectileComponent &projectile) {
        outStream << projectile.speed << " " << projectile.maxTime << " " << projectile.currentTime << " "
            << projectile.originID << " " << projectile.direction.x << " " << projectile.direction.y << " "
            << projectile.direction.z;
        
        return outStream;
    }
}