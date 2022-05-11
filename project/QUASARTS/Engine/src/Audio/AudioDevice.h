#pragma once
#include <AL/alc.h>
#include <AL/al.h>
#include <glm/glm.hpp>

namespace Engine {

	class AudioDevice
	{
	public:
		AudioDevice();
		~AudioDevice();

		//location
		void setPosition(const glm::vec3& pos);
		void setPosition(const float& x, const float& y, const float& z);

		//orientation
		void setOrientation(const float& x, const float& y, const float& z,
			const float& upX, const float& upY, const float& upZ);
		void getOrientation(float& orientation);


		/* Distance model values. */
		//#define AL_INVERSE_DISTANCE                      0xD001
		//#define AL_INVERSE_DISTANCE_CLAMPED              0xD002
		//#define AL_LINEAR_DISTANCE                       0xD003
		//#define AL_LINEAR_DISTANCE_CLAMPED               0xD004
		//#define AL_EXPONENT_DISTANCE                     0xD005
		//#define AL_EXPONENT_DISTANCE_CLAMPED             0xD006
		void setAttunation(ALint model_val);
		
	private:
		ALCdevice* device;
		ALCcontext* context;
	};  
}