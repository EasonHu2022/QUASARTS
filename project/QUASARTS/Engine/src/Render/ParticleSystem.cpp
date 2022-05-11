#include "ParticleSystem.h"


namespace Engine
{
	ParticleSystem::ParticleSystem(float particles, float speed, float grav, float life, float scal, float speedE, float lifeE, float scalE, glm::vec3 dir, float div)
	{
		pps = particles;
		averageSpeed = speed;
		gravity = grav;
		averageLifeLength = life;
		averageScale = scal;
		speedError = speedE;
		lifeError = lifeE;
		scaleError = scalE;
		direction = dir;
		directionDeviation = div;
	}
	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::generateParticle(glm::vec3 center) {
		float deltaT = TimeModule::Instance()->getFrameDeltaTime().sec();
		float particlesToCreate = pps * deltaT;
		float count;
		float partialParticle = modf(particlesToCreate, &count);
		for (int i = 0; i < count; i++) {
			emitParticle(center);
		}
		if ((float)rand() / (float)(RAND_MAX) < partialParticle) {
			emitParticle(center);
		}
	}

	void ParticleSystem::emitParticle(glm::vec3 center) {
		glm::vec3 velocity;
		//if (direction != NULL) {
			velocity = randVecCone(direction, directionDeviation);
		//}
		//else {
			//velocity = randVec();
		//}
		velocity = glm::normalize(velocity);
		velocity = velocity * generateFloat(averageSpeed, speedError);
		float scale = generateFloat(averageScale, scaleError);
		float lifeLength = generateFloat(averageLifeLength, lifeError);
		Engine::Particle particle(center, velocity, gravity, lifeLength, generateRotation(), scale);
		Engine::ParticleMaster::Instance()->addParticle(particle);
	}
	float ParticleSystem::generateFloat(float avg, float error) {
		float offset = ((float)rand() / (float)(RAND_MAX) - 0.5f) * 2.0f * error;
		return avg + offset;
	}
	float ParticleSystem::generateRotation() {
		if (randomRotation) {
			return (float)rand() / (float)(RAND_MAX) * 360.0f;
		}
		else {
			return 0;
		}
	}
	glm::vec3 ParticleSystem::randVecCone(glm::vec3 dir, float angle) {
		float cosAngle = cos(angle);
		float theta = (float)((float)rand() / (float)(RAND_MAX) * 2.0f * 3.14159265358979323846);
		float z = cosAngle + ((float)rand() / (float)(RAND_MAX) * (1 - cosAngle));
		float rootOneMinusZSquared = sqrt(1 - z * z);
		float x = (float)(rootOneMinusZSquared * cos(theta));
		float y = (float)(rootOneMinusZSquared * sin(theta));

		glm::vec4 direction(x, y, z, 1);
		if (dir.x != 0 || dir.y != 0 || (dir.z != 1 && dir.z != -1)) {
			glm::vec3 rotateAxis = glm::cross(dir, glm::vec3(0, 0, 1));
			rotateAxis = glm::normalize(rotateAxis);
			float rotateAngle = (float)acos(glm::dot(dir, glm::vec3(0, 0, 1)));
			glm::mat4 rotationMatrix = glm::mat4(1.0);
			rotationMatrix = glm::rotate(rotationMatrix, -rotateAngle, rotateAxis);
			direction = rotationMatrix * direction;
		}
		else if (dir.z == -1) {
			direction.z *= -1;
		}
		return glm::vec3(direction.x, direction.y, direction.z);
	}
	glm::vec3 ParticleSystem::randVec() {
		float theta = (float)((float)rand() / (float)(RAND_MAX) * 2.0f * 3.14159265358979323846);
		float z = ((float)rand() / (float)(RAND_MAX) * 2.0f) - 1;
		float rootOneMinusZSquared = sqrt(1 - z * z);
		float x = (float)(rootOneMinusZSquared * cos(theta));
		float y = (float)(rootOneMinusZSquared * sin(theta));
		return glm::vec3(x, y, z);
	}
};

