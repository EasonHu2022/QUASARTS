#include "AudioSystem.h"
#include <AL/al.h>


namespace Engine {
	// singleton
	AudioSys* AudioSys::instance = nullptr;

	/// <summary>
	/// Create a singleton of the .
	/// </summary>
	/// <returns> The unique instance of the audio system. </returns>
	AudioSys* AudioSys::Instance()
	{
		if (nullptr == instance)
			instance = new AudioSys();
		return instance;
	}

	/// <summary>
	/// init
	/// </summary>
	void AudioSys::init()
	{
		sound_dev = new SoundDevice();
		sound_src = new SoundSource();
	}

	/// <summary>
	/// start
	/// </summary>
	/// <returns>res</returns>
	int AudioSys::start()
	{
		return 0;
	}

	/// <summary>
	/// update
	/// </summary>
	void AudioSys::update()
	{

	}

	/// <summary>
	/// stop
	/// </summary>
	/// <returns>res</returns>
	int AudioSys::stop()
	{
		return 0;
	}

	/// <summary>
	/// release
	/// </summary>
	void AudioSys::release()
	{
		if (sound_src != nullptr)
		{
			delete sound_src;
			sound_src = nullptr;
		}

		if (sound_dev != nullptr)
		{
			delete sound_dev;
			sound_dev = nullptr;
		}
	}

	void AudioSys::getWorkPath(const std::string& path)
	{
		cur_work_dir = path;
	}

	void AudioSys::playSound(const std::string& name)
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
		
		ALuint audio = SoundBuffer::get()->addSoundEffect(file_path.c_str());
		sound_src->Play(audio);
	}
}