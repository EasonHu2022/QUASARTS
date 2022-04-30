#include "TrackSource.h"
#include "QuasartsEngine.h"
#include <cstddef>
#include <AL/alext.h>
#include <malloc.h>

namespace Engine {

	TrackSource::TrackSource(const char* file_name)
	{
		alGenSources(1, &source);
		alGenBuffers(4, buffers);

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
			QERROR("can't load this clip format: {0} :(", sf_info.channels);
			sf_close(sndfile);
		}

		std::size_t frame_size = ((size_t)8192 * (size_t)sf_info.channels) * sizeof(short);
		buf = static_cast<short*>(malloc(frame_size));
	}

	TrackSource::~TrackSource()
	{
		alDeleteSources(1, &source);

		if (sndfile)
		{
			sf_close(sndfile);
		}
		sndfile = nullptr;
		free(buf);
		alDeleteBuffers(4, buffers);

	}

	void TrackSource::play()
	{		
		alGetError();
		alSourceRewind(source);
		alSourcei(source, AL_BUFFER, 0);
	
		//fill the buffer queue
		ALsizei i;
		for (i = 0; i < 4; i++)
		{
			//get data and fill the buffer
			sf_count_t slen = sf_readf_short(sndfile, buf, 8192);
			if (slen < 1) break;
	
			slen *= sf_info.channels * (sf_count_t)sizeof(short);
			alBufferData(buffers[i], format, buf, (ALsizei)slen, sf_info.samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			QERROR("can't load the track");
		}
	
		/* Now queue and start playback! */
		alSourceQueueBuffers(source, i, buffers);
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

	void TrackSource::isLooping()
	{
		alSourcei(source, AL_LOOPING, (ALint)(is_looping ? 1 : 0));
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
			ALuint buffer_id;
			sf_count_t slen;
		
			//unqueue buffer 
			alSourceUnqueueBuffers(source, 1, &buffer_id);
			processed--;

			//read next block, reset the buffer queue
			slen = sf_readf_short(sndfile, buf, 8192);
			if (slen > 0)
			{
				slen *= sf_info.channels * (sf_count_t)sizeof(short);
				alBufferData(buffer_id, format, buf, (ALsizei)slen, sf_info.samplerate);
				alSourceQueueBuffers(source, 1, &buffer_id);
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
}

