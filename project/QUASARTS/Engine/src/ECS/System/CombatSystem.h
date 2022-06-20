#pragma once

#define PLAYER_CTRL_SHOOT GLFW_MOUSE_BUTTON_LEFT

// Library includes:
#include <cstdlib>
#include <ctime>

// Local includes:
#include "Core/Application.h"
#include "ECS/ECSManager.h"
#include "System.h"
#include "AudioSystem.h"
#include "Time/TimeModule.h"
#include "UISystem.h"
#include "ECS/Showcase-Video.h"

namespace Engine {

    class QS_API CombatSystem : public System
    {
        private:
        // Singleton:
        static CombatSystem* instance;
        CombatSystem();

        public:
        static CombatSystem* Instance();
        ~CombatSystem();

        void init() {}
        int start() { return 0; }
        void update();
        int stop() { return 0; }
        void release() {}

        // Set up all the data required for the component to function:
        void initialize_components() override {}

        void spawn_enemy(EnemySpawnComponent *enemySpawner, unsigned int spawnerID, float moveSpeedMult);
        void shoot(unsigned int origin, unsigned int target, int shooter);

        void kill_planet(unsigned int planetID);
        void destroy_sun(unsigned int sunID);
        void kill_all_enemies();

        unsigned int test_projectile_collision(unsigned int projectileID, bool includeSun);
    };
}