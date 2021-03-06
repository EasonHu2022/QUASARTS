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
		glm::vec2 texOffset1 = glm::vec2(0.0f, 0.0f);
		glm::vec2 texOffset2 = glm::vec2(0.0f, 0.0f);
		float blend;
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
			updateTexCoords();
			time += TimeModule::Instance()->get_frame_delta_time().sec();
			return time < lifetime;
		}

		void updateTexCoords() {

			float blendFactor = time / lifetime;
			int stages = numRows * numRows;
			float progress = blendFactor * (float)stages;
			double first;
			float second = modf(progress, &first);
			int index1 = (int)first;
			int index2 = index1 < stages - 1 ? index1 + 1 : index1;
			blend = second;
			setOffest1(index1);
			setOffest2(index2);
		}

		void setOffest1(int index) {
			int column = index % numRows;
			int row = index / numRows;
			texOffset1.x = (float)column / (float)numRows;
			texOffset1.y = (float)row / (float)numRows;
		}
		void setOffest2(int index) {
			int column = index % numRows;
			int row = index / numRows;
			texOffset2.x = (float)column / (float)numRows;
			texOffset2.y = (float)row / (float)numRows;
		}
	};
};