#include "AudioExporter.h"

void Engine::AudioExporter::exportAudio(sol::state& p_lua_state)
{
	p_lua_state.new_usertype<Audio>("Audio",
		"playClip", &Audio::playClip,
		"pauseClip", &Audio::pauseClip,
		"stopClip", &Audio::stopClip,
		"resumeClip", &Audio::resumeClip,
		"playAllClips", &Audio::playAllClips,
		"setLooping", &Audio::setLooping,
		"playTrack", &Audio::playTrack,
		"pauseTrack", &Audio::pauseTrack,
		"stopTrack", &Audio::stopTrack,
		"resumeTrack", &Audio::resumeTrack,
		"active3DEffect", &Audio::active3DEffect
		);

	p_lua_state.new_usertype<Listener>("Listener",
		"setAttunation", &Listener::setAttunation,
		"setPosition3f", &Listener::setPosition3f,
		"setPositionv", &Listener::setPositionv,
		"setOritentation3f", &Listener::setOritentation3f,
		"setOritentationv", &Listener::setOritentationv
		);
}
