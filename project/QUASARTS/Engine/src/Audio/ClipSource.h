#pragma once
#include <AL/al.h>
#include <glm/glm.hpp>
#include "Core/Core.h"

namespace Engine {

	class QS_API ClipSource
	{
	public:
		ClipSource();
		~ClipSource();
	
		void play(const ALuint buffer);
		bool isPlaying();
		void stop();
		void pause();
		void resume();
		void setLooping(const bool& looping_state);


		void setPosition(const glm::vec3& pos);
		void setPosition(const float& x, const float& y, const float& z);



	private:
		ALuint source;
		ALuint buffers = 0;
		bool is_paused = false;
	};
}

