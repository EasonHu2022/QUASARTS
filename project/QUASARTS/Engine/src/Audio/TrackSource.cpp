#include "TrackSource.h"
#include "QuasartsEngine.h"
#include <cstddef>
#include <AL/alext.h>
#include <malloc.h>

namespace Engine {

	TrackSource::TrackSource()
	{
		alGenSources(1, &source);
		alGenBuffers(4, buffers);
	}

	TrackSource::~TrackSource()
	{
		alDeleteSources(1, &source);

		if (sndfile)
		{
			sf_close(sndfile);
			sndfile = nullptr;
		}
		free(buf);
		alDeleteBuffers(4, buffers);

	}

	void TrackSource::loadTrack(const char* file_name)
	{
		//open the file 
		sndfile = sf_open(file_name, SFM_READ, &sf_info);

		//check if the file can be opened
		if (!sndfile)
		{
			QERROR("Can't open the clip in {0}", file_name);
		}

		//identify the clip format
		if (sf_info.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (sf_info.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (sf_info.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sf_info.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format)
		{
			QERROR("can't load this track format: {0} :(, please use .wav", sf_info.channels);
			sf_close(sndfile);
		}

		std::size_t frame_size = ((size_t)8192 * (size_t)sf_info.channels) * sizeof(short);
		buf = static_cast<short*>(malloc(frame_size));
	}

	void TrackSource::play()
	{		
		alGetError();
		alSourceRewind(source);
		alSourcei(source, AL_BUFFER, 0);
	
		//fill the buffer queue
		ALsizei idx;
		for (idx = 0; idx < 4; idx++)
		{
			//get data and fill the buffer
			sf_count_t slen = sf_readf_short(sndfile, buf, 8192);
			if (slen > 0)
			{
				slen *= sf_info.channels * (sf_count_t)sizeof(short);
				alBufferData(buffers[idx], format, buf, (ALsizei)slen, sf_info.samplerate);
			}	
		}
		if (alGetError() != AL_NO_ERROR)
		{
			QERROR("can't load the track");
		}
	
		//get queue and play buffers
		alSourceQueueBuffers(source, idx, buffers);
		alSourcePlay(source);
		if (alGetError() != AL_NO_ERROR)
		{
			QERROR("can't play the track");
		}
	
	}

	bool TrackSource::isPlaying()
	{
		ALint state = AL_PLAYING;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	void TrackSource::stop()
	{
		alSourceStop(source);
	}

	void TrackSource::pause()
	{
		alSourcePause(source);
		is_paused = true;
	}

	void TrackSource::resume()
	{
		alSourcePlay(source);
		is_paused = false;
	}

	void TrackSource::setLooping(const bool& looping_state)
	{
		alSourcei(source, AL_LOOPING, (ALint)looping_state);
	}
		
	void TrackSource::updateBuffer()
	{

		alGetError();
		ALint processed;
		ALint state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);	
	
		while (processed > 0)
		{
			ALuint idx;
		
			//unqueue buffer 
			alSourceUnqueueBuffers(source, 1, &idx);
			processed--;

			//read next block, reset the buffer queue
			sf_count_t slen = sf_readf_short(sndfile, buf, 8192);
			if (slen > 0)
			{
				slen *= sf_info.channels * (sf_count_t)sizeof(short);
				alBufferData(idx, format, buf, (ALsizei)slen, sf_info.samplerate);
				alSourceQueueBuffers(source, 1, &idx);
			}
		}
	
		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			//check queue state
			ALint queued;
			alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0)
			{
				return;
			}
			alSourcePlay(source);
		}	
	}

	void TrackSource::setPosition(const glm::vec3& pos)
	{
		alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	}

	void TrackSource::setPosition(const float& x, const float& y, const float& z)
	{
		alSource3f(source, AL_POSITION, x, y, z);
	}

}

