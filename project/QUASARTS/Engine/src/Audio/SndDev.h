#pragma once
#include <AL/alc.h>

class SoundDevice
{
public:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

};