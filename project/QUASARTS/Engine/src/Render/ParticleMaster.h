#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Particle.h"
#include "ParticleRenderer.h"
namespace Engine
{
    class ParticleMaster
    {
    private:
        std::vector<Particle> particles;
        ParticleRenderer* renderer;

    public:
        ParticleMaster() {}
        ~ParticleMaster() {}
        void init(RenderContext* renderContext) {
            renderer = new ParticleRenderer(renderContext);
            renderer->init();
            printf("update");
        }
        void update() {
            for (int i = 0; i < particles.size(); i++) {
                bool alive = particles[i].update();
                if (!alive)
                    particles.erase(particles.begin() + i);
            }
        }
        void render() {
            renderer->render(particles);
        }
        void release() {
            renderer->release();
        }
        void addParticle(Particle particle) {
            particles.push_back(particle);
        }
    };

};
