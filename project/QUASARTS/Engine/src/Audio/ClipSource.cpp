#include "ClipSource.h"

namespace Engine {

	ClipSource::ClipSource()
	{
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, 0);

		//TODO 3D
		//alSourcef(p_Source, AL_PITCH, p_Pitch);
		//alSourcef(p_Source, AL_GAIN, p_Gain);
		//alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
		//alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
		//alSourcei(p_Source, AL_LOOPING, p_LoopSound);
	}
	
	ClipSource::~ClipSource()
	{
		alDeleteSources(1, &source);
	}
	
	void ClipSource::play(const ALuint buffer)
	{
		if (buffers != buffer)
		{
			buffers = buffer;
			alSourcei(source, AL_BUFFER, (ALint)buffers);
		}
		alSourcePlay(source);
	}

	bool ClipSource::isPlaying()
	{
		ALint state = AL_PLAYING;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}
	
	void ClipSource::stop()
	{
		alSourceStop(source);
	}
	
	void ClipSource::pause()
	{
		alSourcePause(source);
		is_paused = true;
	}
	
	void ClipSource::resume()
	{
		alSourcePlay(source);
		is_paused = false;
	}

	void ClipSource::isLooping()
	{
		alSourcei(source, AL_LOOPING, (ALint)(is_looping? 1:0));
	}
}
