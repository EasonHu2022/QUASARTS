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
}