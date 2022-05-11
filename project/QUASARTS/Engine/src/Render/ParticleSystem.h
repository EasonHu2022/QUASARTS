#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <math.h>
#include "ParticleMaster.h"
#include "Time/TimeModule.h"

namespace Engine
{
    class QS_API ParticleSystem
    {
    public:
        float pps, averageSpeed, gravity, averageLifeLength, averageScale;

        float speedError, lifeError, scaleError = 0;
        bool randomRotation = false;
        glm::vec3 direction;
        float directionDeviation = 0;

        ParticleSystem(float, float, float, float, float, float, float, float, glm::vec3, float);
        ~ParticleSystem();
        
        void generateParticle(glm::vec3);

    private:
        void emitParticle(glm::vec3);
        float generateFloat(float, float);
        float generateRotation();
        static glm::vec3 randVecCone(glm::vec3, float);
        glm::vec3 randVec();
    };

};
