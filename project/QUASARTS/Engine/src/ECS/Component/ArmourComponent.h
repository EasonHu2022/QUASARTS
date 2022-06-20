#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API ArmourComponent {
        float armour_value;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, ArmourComponent &armour) {
        inStream >> armour.armour_value;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const ArmourComponent &armour) {
        outStream << armour.armour_value;
        
        return outStream;
    }
}