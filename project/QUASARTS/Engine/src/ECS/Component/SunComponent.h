#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API SunComponent {
        float max_health;
        float current_health;
        float armour;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, SunComponent &sun) {
        inStream >> sun.max_health >> sun.current_health >> sun.armour;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const SunComponent &sun) {
        outStream << sun.max_health << " " << sun.current_health << " " << sun.armour;
        
        return outStream;
    }
}