#include "AudioExporter.h"

void Engine::AudioExporter::exportAudio(sol::state& p_lua_state)
{
	p_lua_state.new_usertype<Audio>("Audio",
		"playClip", &Audio::playClip,
		"pauseClip", &Audio::pauseClip,
		"stopClip", &Audio::stopClip,
		"resumeClip", &Audio::resumeClip,
		"playAllClips", &Audio::playAllClips,
		"playTrack", &Audio::playTrack,
		"pauseTrack", &Audio::pauseTrack,
		"stopTrack", &Audio::stopTrack,
		"resumeTrack", &Audio::resumeTrack
		);
}
