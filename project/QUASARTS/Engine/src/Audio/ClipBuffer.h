#pragma once
#include <AL/al.h>
#include <vector>

namespace Engine {

	class ClipBuffer
	{
	public:
		ClipBuffer();
		~ClipBuffer();

		ALuint loadSoundClip(const char* file_name);
		bool removeSoundClip(const ALuint& buffer);
		std::vector<ALuint> getAllClips();
	
	private:
		std::vector<ALuint> clips;   //get all clips
	};
}