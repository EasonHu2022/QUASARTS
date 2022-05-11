#pragma once

// Local includes:
#include "ECS/ECSManager.h"
#include "System.h"

namespace Engine {

    class QS_API ParticleSystem : public System
    {

    public:
        ParticleSystem();
        ~ParticleSystem();

        // Called when ENGINE starts.
        void init() {}
        // Called when GAME starts.
        int start() { return 0; }
        // Called for each GAME update.
        void update();
        // Called when GAME stops.
        int stop() { return 0; }
        void release() {}


        // Usage //
    public:
        
    private:
        

    };

}