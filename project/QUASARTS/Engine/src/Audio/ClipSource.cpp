#include "ClipSource.h"

namespace Engine {

	ClipSource::ClipSource()
	{
		alGenSources(1, &source);
		alSourcei(source, AL_BUFFER, 0);
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
	}
	
	void ClipSource::resume()
	{
		alSourcePlay(source);
	}

	void ClipSource::setLooping(const bool& looping_state)
	{
		alSourcei(source, AL_LOOPING, (ALint)looping_state);
	}

	void ClipSource::setPosition(const glm::vec3& pos)
	{
		alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	}

	void ClipSource::setPosition(const float& x, const float& y, const float& z)
	{
		alSource3f(source, AL_POSITION, x, y, z);
	}
}
