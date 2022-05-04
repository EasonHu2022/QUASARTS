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

	};
}