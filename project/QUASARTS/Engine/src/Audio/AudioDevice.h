#pragma once
#include <AL/alc.h>

namespace Engine {

	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();
		
	private:
		ALCdevice* device;
		ALCcontext* context;
	};  
}