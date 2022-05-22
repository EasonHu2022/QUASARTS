#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API WeaponComponent {
        float base_damage;
        float attack_range;
        float attack_speed;
        float lastFire = 0.0;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, WeaponComponent &weapon) {
        inStream >> weapon.base_damage >> weapon.attack_range >> weapon.attack_speed >> weapon.lastFire;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const WeaponComponent &weapon) {
        outStream << weapon.base_damage << " " << weapon.attack_range << " " << weapon.attack_speed << " " << weapon.lastFire;
        
        return outStream;
    }
}