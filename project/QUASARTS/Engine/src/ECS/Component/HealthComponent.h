#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API HealthComponent {
        float max_health;
        float current_health;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, HealthComponent &health) {
        inStream >> health.max_health >> health.current_health;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const HealthComponent &health) {
        outStream << health.max_health << " " << health.current_health;
        
        return outStream;
    }
}