#pragma once
#include <AL/al.h>
#include <sndfile.h>
#include "Core/Core.h"

namespace Engine {

	class QS_API TrackSource
	{
	public:	
		TrackSource(const char* file_name);
		~TrackSource();

		void play();
		bool isPlaying();
		void stop();
		void pause();
		void resume();
		void isLooping();	
		void updateBuffer();	
	
	private:
		TrackSource() = delete;
	
	private:
		ALuint source;
		ALuint buffers[4];
		SNDFILE* sndfile;
		SF_INFO sf_info;
		short* buf;
		ALenum format;
		bool is_paused = false;
		bool is_looping = true;
	};
}

