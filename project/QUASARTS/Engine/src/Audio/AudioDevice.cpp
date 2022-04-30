#include "AudioDevice.h"
#include "QuasartsEngine.h"
#include <AL/al.h>

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
}

