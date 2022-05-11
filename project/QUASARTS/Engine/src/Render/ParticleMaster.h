#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Particle.h"
#include "ParticleRenderer.h"
#include "Time/TimeModule.h"

namespace Engine
{
    class QS_API ParticleMaster
    {
    private:
        std::vector<Particle> particles;
        ParticleRenderer* renderer;
        static ParticleMaster* instance;

    public:
        QTime counter;
        static ParticleMaster* Instance();
        ParticleMaster();
        ~ParticleMaster();
        void init(RenderContext* renderContext);
        void update();
        void render();
        void release();
        void addParticle(Particle particle);

    };

};
