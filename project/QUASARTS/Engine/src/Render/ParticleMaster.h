#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Particle.h"
#include "ParticleRenderer.h"
#include "ParticleTexture.h"
#include "Time/TimeModule.h"

namespace Engine
{
    class QS_API ParticleMaster
    {
    private:
        std::map<std::string, std::pair<ParticleTexture, std::vector<Particle>>> emitters;
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
        void addParticle(std::string texName, ParticleTexture tex, Particle particle);

    };

};
