#include "AudioDevice.h"
#include "QuasartsEngine.h"
#include <AL/al.h>
#include <vector>

namespace Engine {

	AudioDevice::AudioDevice()
	{
		// get device
		device = alcOpenDevice(nullptr); 
		if (!device) {
			QWARN("failed to get device");
		}

		// create context
		context = alcCreateContext(device, nullptr);  
		if (!context) {
			QWARN("failed to create context");
		}
		
		// make context current
		alcMakeContextCurrent(context);
	}
	
	AudioDevice::~AudioDevice()
	{
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void AudioDevice::setPosition(const glm::vec3& pos)
	{
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	}


	void AudioDevice::setPosition(const float& x, const float& y, const float& z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}



	void AudioDevice::setOrientation(const float& x, const float& y, const float& z)
	{
		std::vector<float> orientation;
		orientation.push_back(x);
		orientation.push_back(y);
		orientation.push_back(z);
		orientation.push_back(0.f);
		orientation.push_back(0.f);
		orientation.push_back(1.f);
		alListenerfv(AL_ORIENTATION, orientation.data());
	}

	void AudioDevice::setAttunation(ALint model_val)
	{
		alDistanceModel(model_val);
	}


}

