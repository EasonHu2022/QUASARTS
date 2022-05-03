#include "ClipBuffer.h"
#include "QuasartsEngine.h"
#include <sndfile.h>
#include <inttypes.h>
#include <cstdint>
#include <AL/alext.h>


namespace Engine {
	
	ClipBuffer::ClipBuffer()
	{
		clips.clear();
	}

	ClipBuffer::~ClipBuffer()
	{
		alDeleteBuffers(clips.size(), clips.data());
	}

	ALuint ClipBuffer::loadSoundClip(const char* file_name)
	{
		//open the file 
		SF_INFO sf_info;
		SNDFILE* sndfile = sf_open(file_name, SFM_READ, &sf_info);
		//check if the file can be opened
		if (!sndfile)
		{
			QERROR("Can't open the clip in {0}", file_name);
			return 0;
		}			
		//identify the clip format
		ALenum format = AL_NONE;  // init format
		if (sf_info.channels == 1)
			format = AL_FORMAT_MONO16; // mono audio
		else if (sf_info.channels == 2)
			format = AL_FORMAT_STEREO16; // stereo audio
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
			return 0;
		}
		//decode the clip and transfer to buffer
		short* buf;
		buf = static_cast<short*>(malloc((size_t)(sf_info.frames * sf_info.channels) * sizeof(short)));
		sf_count_t num_frames = sf_readf_short(sndfile, buf, sf_info.frames);
		ALsizei num_bytes = (ALsizei)(num_frames * sf_info.channels) * (ALsizei)sizeof(short);
		
		//transfer the buffer and then close the file
		ALuint buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, buf, num_bytes, sf_info.samplerate);
		free(buf);
		sf_close(sndfile);
	
		ALenum error_msg = alGetError();
		if (error_msg != AL_NO_ERROR)
		{
			QERROR("OpenAL Error: {0}", alGetString(error_msg));
			if (buffer && alIsBuffer(buffer))
			{
				alDeleteBuffers(1, &buffer);
			}
			return 0;
		}
	
		clips.push_back(buffer);  // add to the list of known buffers
		return buffer;
	}
	
	bool ClipBuffer::removeSoundClip(const ALuint& buffer)
	{
		auto it = clips.begin();
		while (it != clips.end())
		{
			if (*it == buffer)
			{
				alDeleteBuffers(1, &*it);	
				it = clips.erase(it);	
				return true;
			}
			else
			{
				++it;
			}
		}
		QWARN("There is no clip to remove");
		return false;
	}
	std::vector<ALuint> ClipBuffer::getAllClips()
	{
		return clips;
	}
}