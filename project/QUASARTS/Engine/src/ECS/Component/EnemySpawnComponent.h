#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    struct QS_API EnemySpawnComponent {
        float spawnRate = 1.0;
        float lastSpawn = 0.0;
        unsigned int maxSpawns = 1;
        unsigned int currentSpawns = 0;
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, EnemySpawnComponent &enemySpawn) {
        inStream >> enemySpawn.spawnRate >> enemySpawn.lastSpawn >> enemySpawn.maxSpawns >> enemySpawn.currentSpawns;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const EnemySpawnComponent &enemySpawn) {
        outStream << enemySpawn.spawnRate << " " << enemySpawn.lastSpawn << " " << enemySpawn.maxSpawns << " " << enemySpawn.currentSpawns;
        
        return outStream;
    }
}