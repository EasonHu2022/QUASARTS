#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API EnemyComponent {
        float moveSpeedMult = 1.0;
        unsigned int spawnerID = 0;
        unsigned int targetID = TOO_MANY_ENTITIES;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, EnemyComponent &enemy) {
        inStream >> enemy.moveSpeedMult >> enemy.spawnerID >> enemy.targetID;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const EnemyComponent &enemy) {
        outStream << enemy.moveSpeedMult << " " << enemy.spawnerID << " " << enemy.targetID;
        
        return outStream;
    }
}