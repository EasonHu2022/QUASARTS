#pragma once

// Library includes:

#include <string>
// Local includes:
#include "ECS/ECS-Common.h"
#include "glm/glm.hpp"
#include "Render/ParticleMaster.h"

namespace Engine {
	struct QS_API ParticleComponent
	{
		float pps, speed, gravity, averageLifeLength, averageScale;

		float speedError, lifeError, scaleError = 0;
		bool randomRotation = false, is_on = false;
		glm::vec3 direction;
		float directionDeviation = 0;
	};

	// Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, ParticleComponent &particle) {
        inStream >> particle.pps >> particle.speed >> particle.gravity
			>> particle.averageLifeLength >> particle.averageScale >> particle.speedError
			>> particle.lifeError >> particle.scaleError >> particle.randomRotation
			>> particle.is_on >> particle.direction[0] >> particle.direction[1]
			>> particle.direction[2] >> particle.directionDeviation;

        return inStream;
    }

	// Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const ParticleComponent &particle) {
        outStream << particle.pps << " " << particle.speed << " " << particle.gravity
			<< " " << particle.averageLifeLength << " " << particle.averageScale << " "
			<< particle.speedError << " " << particle.lifeError << " " << particle.scaleError
			<< " " << particle.randomRotation << " " << particle.is_on << " "
			<< particle.direction[0] << " " << particle.direction[1] << " "
			<< particle.direction[2] << " " << particle.directionDeviation;
        
        return outStream;
    }
}