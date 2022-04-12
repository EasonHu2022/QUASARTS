#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "ECS/System/System.h"
#include "ECS/ECSManager.h"

namespace Engine {
    class QS_API ExampleSystem : public System {
        private:
        // Singleton:
        static ExampleSystem *instance;
        ExampleSystem();

        public:
        static ExampleSystem *Instance();
        ~ExampleSystem();

        // Functions inherited from IManager:
        void init();
		int start();
		void update();
		int stop();
		void release();
    };
}