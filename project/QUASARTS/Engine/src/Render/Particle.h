#pragma once
#include "Core/Core.h"
#include <string>
#include "glm/glm.hpp"
#include "Time/TimeModule.h"
#include "Texture2D.h"
#include <map>

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

		int numRows;
	public:
		Particle(int rows, glm::vec3 pos, glm::vec3 vel, float grav, float life, float rot, float scal);
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

		int getRows() {
			return numRows;
		}

		bool update() {
			velocity.y += gravity * TimeModule::Instance()->get_frame_delta_time().sec();
			glm::vec3 change = velocity;
			change = (float)TimeModule::Instance()->get_frame_delta_time().sec() * change;
			position = position + change;
			time += TimeModule::Instance()->get_frame_delta_time().sec();
			return time < lifetime;
		}
	};
};