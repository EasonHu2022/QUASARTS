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
		float pps, averageSpeed, gravity, averageLifeLength, averageScale;

		float speedError, lifeError, scaleError = 0, posError;
		bool randomRotation = false, is_on = true, cone = true;
		glm::vec3 direction;
		float directionDeviation = 0;
		ParticleTexture texture;
		std::string path = "";
		ParticleComponent() {
			pps = 90.0f;
			averageSpeed = 3.0f;
			gravity = 1.0f;
			averageLifeLength = 2.0f;
			averageScale = 0.1f;
			speedError = 0.3f;
			lifeError = 0.2f;
			scaleError = 0.01f;
			direction = glm::vec3(0.0f, 1.0f, 0.0f);
			directionDeviation = 0.5f;
			posError = 0.0f;
		}

		void generateParticle(glm::vec3 center) {
			if (texture.rows > 0) {
				float deltaT = TimeModule::Instance()->get_frame_delta_time().sec();
				float particlesToCreate = pps * deltaT;
				double count;
				float partialParticle = modf(particlesToCreate, &count);
				for (int i = 0; i < count; i++) {
					emitParticle(center);
				}
				if ((float)rand() / (float)(RAND_MAX) < partialParticle) {
					emitParticle(center);
				}
			}
		}

		void emitParticle(glm::vec3 center) {
			glm::vec3 velocity;
			if (cone) {
			velocity = randVecCone(direction, directionDeviation);
			}
			else {
				velocity = randVec();
			}
			velocity = glm::normalize(velocity);
			velocity = velocity * generateFloat(averageSpeed, speedError);
			float scale = generateFloat(averageScale, scaleError);
			float lifeLength = generateFloat(averageLifeLength, lifeError);
			Engine::Particle particle(texture.rows, center, velocity, gravity, lifeLength, generateRotation(), scale);
			//Texture2D to_add = *texture;
			Engine::ParticleMaster::Instance()->addParticle(path, texture, particle);
		}

		float generateFloat(float avg, float error) {
			float offset = ((float)rand() / (float)(RAND_MAX)-0.5f) * 2.0f * error;
			return avg + offset;
		}

		glm::vec3 generateVec(glm::vec3 avg, float error) {
			float x = ((float)rand() / (float)(RAND_MAX)-0.5f) * 2.0f * error;
			float z = ((float)rand() / (float)(RAND_MAX)-0.5f) * 2.0f * error;
			return glm::vec3(avg.x + x, avg.y, avg.z + z);
		}

		float generateRotation() {
			if (randomRotation) {
				return (float)rand() / (float)(RAND_MAX) * 360.0f;
			}
			else {
				return 0;
			}
		}

		glm::vec3 randVecCone(glm::vec3 dir, float angle) {
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

		glm::vec3 randVec() {
			float theta = (float)((float)rand() / (float)(RAND_MAX) * 2.0f * 3.14159265358979323846);
			float z = ((float)rand() / (float)(RAND_MAX) * 2.0f) - 1;
			float rootOneMinusZSquared = sqrt(1 - z * z);
			float x = (float)(rootOneMinusZSquared * cos(theta));
			float y = (float)(rootOneMinusZSquared * sin(theta));
			return glm::vec3(x, y, z);
		}

		void loadtex(std::string texPath) {
			if (texture.rows == -1) {
				texture.rows = 1;
				texture.texture = new Texture2D(texPath);
				path = texPath;
			}
			else {
				delete texture.texture;
				texture.texture = new Texture2D(texPath);
			}
		}
	};

	// Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, ParticleComponent &particle) {
        inStream >> particle.pps >> particle.averageSpeed >> particle.gravity
			>> particle.averageLifeLength >> particle.averageScale >> particle.speedError
			>> particle.lifeError >> particle.scaleError >> particle.randomRotation
			>> particle.is_on >> particle.direction[0] >> particle.direction[1]
			>> particle.direction[2] >> particle.directionDeviation;

        return inStream;
    }

	// Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const ParticleComponent &particle) {
        outStream << particle.pps << " " << particle.averageSpeed << " " << particle.gravity
			<< " " << particle.averageLifeLength << " " << particle.averageScale << " "
			<< particle.speedError << " " << particle.lifeError << " " << particle.scaleError
			<< " " << particle.randomRotation << " " << particle.is_on << " "
			<< particle.direction[0] << " " << particle.direction[1] << " "
			<< particle.direction[2] << " " << particle.directionDeviation;
        
        return outStream;
    }
}