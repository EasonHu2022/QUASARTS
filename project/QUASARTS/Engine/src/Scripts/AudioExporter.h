#pragma once
#include <string>
#include "QuasartsEngine.h"

namespace Engine {

	class AudioExporter
	{
	public:
		//export log function
		static void exportAudio(sol::state& p_lua_state);
	};

	//enable to play audio in lua side
	class Audio
	{
	public:

		//sound clip
		static void playClip(const std::string& name)
		{
			Engine::AudioSystem::Instance()->playSoundClip(name);
		}

		static void stopClip()
		{
			Engine::AudioSystem::Instance()->stopClip();
		}

		static void pauseClip()
		{
			Engine::AudioSystem::Instance()->pauseClip();
		}

		static void resumeClip()
		{
			Engine::AudioSystem::Instance()->resumeClip();
		}

		static void playAllClips()
		{
			Engine::AudioSystem::Instance()->playAllClips();
		}

		static void setLooping(const bool& loop_state)
		{
			return Engine::AudioSystem::Instance()->getClipSource()->setLooping(loop_state);
		}

		//long track
		static void playTrack(const std::string& name)
		{
			Engine::AudioSystem::Instance()->playTrack(name);
		}

		static void stopTrack()
		{
			Engine::AudioSystem::Instance()->stopTrack();
		}

		static void pauseTrack()
		{
			Engine::AudioSystem::Instance()->pauseTrack();
		}

		static void resumeTrack()
		{
			Engine::AudioSystem::Instance()->resumeTrack();
		}

		//3D effect
		static void active3DEffect(const glm::vec3& pos)
		{
			auto src = Engine::AudioSystem::Instance()->getClipSource();
			src->setPosition(pos);
		}

	};
	//device or listener
	class Listener
	{
	public:

		static void setAttunation()
		{
			auto device = Engine::AudioSystem::Instance()->getDevice();
			device->setAttunation(AL_INVERSE_DISTANCE_CLAMPED);
		}

		static void setPosition3f(const float& x, const float& y, const float& z)
		{
			auto device = Engine::AudioSystem::Instance()->getDevice();
			device->setPosition(x, y, z);
		}

		static void setPositionv(const glm::vec3& v)
		{
			auto device = Engine::AudioSystem::Instance()->getDevice();
			device->setPosition(v);
		}

		static void setOritentation(const float& x, const float& y, const float& z)
		{
			auto device = Engine::AudioSystem::Instance()->getDevice();
			device->setOrientation(x, y, z);
		}

	};
}