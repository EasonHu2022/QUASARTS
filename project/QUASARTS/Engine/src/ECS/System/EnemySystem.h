#pragma once

// Library includes:
#include <cstdlib>
#include <ctime>

// Local includes:
#include "ECS/ECSManager.h"
#include "System.h"
#include "CollisionSystem.h"
#include "Time/TimeModule.h"

namespace Engine {

    class QS_API EnemySystem : public System
    {
        private:
        // Singleton:
        static EnemySystem* instance;
        EnemySystem();

        public:
        static EnemySystem* Instance();
        ~EnemySystem();

        void init() {}
        int start() { return 0; }
        void update();
        int stop() { return 0; }
        void release() {}

        // Set up all the data required for the component to function:
        void initialize_components() override {}

        void spawn_enemy(EnemySpawnComponent *enemySpawner, unsigned int spawnerID,
                                                                float moveSpeedMult);
        
        void shoot(unsigned int origin, unsigned int target, bool shootAtPlanet);

        void kill_planet(unsigned int planetID);
    };
}