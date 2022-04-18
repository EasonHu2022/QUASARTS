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
		sound_dev = SoundDevice::get();
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
	}

	void AudioSys::playSound(const std::string& name)
	{
		std::string file_path = "..\\Assets\\Audio\\" + name + ".ogg";
		ALuint audio = SoundBuffer::get()->addSoundEffect(file_path.c_str());
		SoundSource src;
		src.Play(audio);
	}
}