#include "AudioSystem.h"
#include <AL/al.h>

namespace Engine {

	// singleton
	AudioSystem* AudioSystem::instance = nullptr;

	/// <summary>
	/// Create a singleton of the .
	/// </summary>
	/// <returns> The unique instance of the audio system. </returns>
	AudioSystem* AudioSystem::Instance()
	{
		if (nullptr == instance)
			instance = new AudioSystem();
		return instance;
	}

	/// <summary>
	/// init
	/// </summary>
	void AudioSystem::init()
	{
		audio_dev = new AudioDevice(); // init the audio device
		clip_buffer = new ClipBuffer();
		clip_src = new ClipSource();
		
	}

	/// <summary>
	/// start
	/// </summary>
	/// <returns>res</returns>
	int AudioSystem::start()
	{
		return 0;
	}

	/// <summary>
	/// update
	/// </summary>
	void AudioSystem::update()
	{

	}

	/// <summary>
	/// stop
	/// </summary>
	/// <returns>res</returns>
	int AudioSystem::stop()
	{
		return 0;
	}

	/// <summary>
	/// release
	/// </summary>
	void AudioSystem::release()
	{

		if (clip_src != nullptr)
		{
			delete clip_src;
			clip_src = nullptr;
		}

		if (audio_dev != nullptr)
		{
			delete audio_dev;
			audio_dev = nullptr;
		}

		if (clip_buffer != nullptr)
		{
			delete clip_buffer;
			clip_buffer = nullptr;
		}

		if (track_src != nullptr)
		{
			delete track_src;
			track_src = nullptr;
		}
	}

	void AudioSystem::getWorkPath(const std::string& path)
	{
		cur_work_dir = path;
	}

	void AudioSystem::playSoundClip(const std::string& name)
	{

		std::string file_path;
		if (cur_work_dir.empty())
		{
			file_path = "..\\Assets\\Audio\\" + name + ".ogg";
		}
		else
		{
			auto temp = cur_work_dir.substr(0, cur_work_dir.find_last_of("E"));
			file_path = temp + "\\Assets\\Audio\\" + name + ".ogg";
		}
		
		ALuint clip = clip_buffer->loadSoundClip(file_path.c_str());
		clip_src->play(clip);

	}
	void AudioSystem::stopClip()
	{
		this->clip_src->stop();
	}
	void AudioSystem::pauseClip()
	{
		this->clip_src->pause();
	}
	void AudioSystem::resumeClip()
	{
		this->clip_src->resume();
	}
	void AudioSystem::playTrack()
	{
		TrackSource buf("..\\Assets\\Audio\\TownTheme.wav");
		buf.play();
		while (buf.isPlaying())
		{
			buf.updateBuffer();
		}
	}
	void AudioSystem::playTrack(const std::string& name)
	{
		std::string file_path = "..\\Assets\\Audio\\" + name + ".wav";
		TrackSource buf(file_path.c_str());
		buf.play();
		while (buf.isPlaying())
		{
			buf.updateBuffer();
		}

	}
	void AudioSystem::stopTrack()
	{
		this->track_src->stop();
	}
	void AudioSystem::pauseTrack()
	{
		this->track_src->pause();
	}
	void AudioSystem::resumeTrack()
	{
		this->track_src->resume();
	}
}