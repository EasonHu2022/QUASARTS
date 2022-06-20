#pragma once
#include "Core/Core.h"
#include "ECSManager.h"
#include "ECS-Common.h"
#include "Time/TimeModule.h"

#include "System/CameraSystem.h"
#include "System/ScriptSystem.h"
#include "System/CollisionSystem.h"
#include "System/CombatSystem.h"

#include "Entity-Stress-Test.h"

#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Engine
{
    class QS_API ShowcaseVideo : public IManager
    {
        private:
        // Singleton:
        static ShowcaseVideo *instance;
        ShowcaseVideo() {
            // Video intro:
            video_intro_running = false;

            // Script test:
            script_test_running = false;

            // Level demo:
            level_demo_running = false;
        }

        public:
        static ShowcaseVideo *Instance();

        ~ShowcaseVideo();

        // Functions inherited from IManager:
        void init() {}
		int start() { return 0; }
		void update();
		int stop() { return 0; }
		void release() {}

        void create_video_intro();
        void create_quasarts_logo();
        void update_camera_intro();

        void create_script_test(int option);
        void update_script_test();
        void destroy_sphere(unsigned int entityID);

        void give_player_weapon();

        void start_level_demo();
        void update_level_demo();

        void spawn_enemy_spawner(glm::vec3 position, glm::vec3 rotation, int maxSpawns);
        void create_UI_element(std::string uiTexPath, bool uiTransparency, int priority, float totalLifetime);
        int count_planets();

        private:
        // Video intro:
        bool video_intro_running;

        // Script test:
        unsigned int moonID;
        unsigned int earthID;
        bool script_test_running;

        // Level demo:
        bool level_demo_running;
    };
}