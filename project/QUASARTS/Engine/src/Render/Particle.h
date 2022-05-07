#pragma once
#include "Core/Core.h"
#include <string>
#include "glm/glm.hpp"
#include "Time/TimeModule.h"

namespace Engine
{
	class QS_API Particle
	{
	private:
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };
		float gravity;
		float lifetime;
		float rotation;
		float scale;

		float time = 0;
	public:
		Particle(glm::vec3 pos, glm::vec3 vel, float grav, float life, float rot, float scal);
		~Particle();
		
		glm::vec3 getPosition() {
			return position;
		}

		float getRotation() {
			return rotation;
		}

		float getScale() {
			return scale;
		}

		bool update() {
			velocity.y += TimeModule::Instance()->getFrameDeltaTime().sec();
			glm::vec3 change = velocity;
			change = (float)TimeModule::Instance()->getFrameDeltaTime().sec() * change;
			position = position + change;
			time += TimeModule::Instance()->getFrameDeltaTime().sec();
			return time < lifetime;
		}
	};
};