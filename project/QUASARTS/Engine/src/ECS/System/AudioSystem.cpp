#include "AudioSystem.h"
#include "ResourceManager/FileModule.h"
#include <AL/al.h>
#include "QuasartsEngine.h"

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
		track_src = new TrackSource();

		ALint attunation = AL_INVERSE_DISTANCE_CLAMPED;
		audio_dev->setAttunation(attunation);
		audio_dev->setPosition(0.f, 0.f, 0.f);
		audio_dev->setOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

		//clip_src->isLooping();
		//playSoundClip("sci-fidrone");
		//clip_src->setPosition(0.f, 5.f, 0.f);

		//playTrack("TownTheme");
		//track_src->setPosition(3.f, 5.f, 0.f);


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
		if (track_src != nullptr)
		{
			track_src->updateBuffer();
		}


		//test3D();
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

	void AudioSystem::playSoundClip(const std::string& name)
	{

		//get default engine assets path
		std::string path = FileModule::Instance()->get_internal_assets_path();
		std::string file_path = path + "Audio/" + name + ".ogg";

		ALuint clip = clip_buffer->loadSoundClip(file_path.c_str());
		clip_src->play(clip);

	}
	void AudioSystem::stopClip()
	{
		if (clip_src != nullptr)
		{
			clip_src->stop();
		}
	}
	void AudioSystem::pauseClip()
	{
		if (clip_src != nullptr)
		{
			clip_src->pause();
		}
	}
	void AudioSystem::resumeClip()
	{
		if (clip_src != nullptr)
		{
			clip_src->resume();
		}
	}
	void AudioSystem::playAllClips()
	{
		auto buffers = clip_buffer->getAllClips();
		for (auto it : buffers)
		{
			clip_src->play(it);
		}
	}

	ClipSource* AudioSystem::getClipSource()
	{
		return clip_src;
	}

	void AudioSystem::playTrack(const std::string& name)
	{
		//get default engine assets path
		std::string path = FileModule::Instance()->get_internal_assets_path();

		std::string file_path = path + "Audio/" + name + ".wav";

		track_src->loadTrack(file_path.c_str());
		track_src->play();
	}
	void AudioSystem::stopTrack()
	{
		if (track_src != nullptr)
		{
			track_src->stop();
		}
	} 
	void AudioSystem::pauseTrack()
	{
		if (track_src != nullptr)
		{
			track_src->pause();
		}
	}
	void AudioSystem::resumeTrack()
	{
		if (track_src != nullptr)
		{
			track_src->resume();
		}
	}

	TrackSource* AudioSystem::getTrackSource()
	{
		return track_src;
	}

	AudioDevice* AudioSystem::getDevice()
	{
		return audio_dev;
	}


	glm::vec3 pos = { 0,0,0 };
	void AudioSystem::test3D()
	{

		if (Engine::Input::get_key_pressed(Q_KEY_W))
		{
			audio_dev->setOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
			pos.y += 1.f;
		}

		if (Engine::Input::get_key_released(Q_KEY_D))
		{
			audio_dev->setOrientation(1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
			pos.x += 1.f;
		}

		if (Engine::Input::get_key_released(Q_KEY_A))
		{
			audio_dev->setOrientation(-1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
			pos.x -= 1.f;
		}

		if (Engine::Input::get_key_released(Q_KEY_S))
		{
			audio_dev->setOrientation(0.f, -1.f, 0.f, 0.f, 0.f, 1.f);
			pos.y -= 1.f;
		}

		QWARN("pos.x: {0},  pos.y: {1}", pos.x, pos.y);
		audio_dev->setPosition(pos);

	}
}