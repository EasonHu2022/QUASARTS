#pragma once
#include "Core/IManager.h"

#include "SndBuffer.h"
#include "SndDev.h"
#include "SndSrc.h"
#include <string>

namespace Engine {

	class QS_API AudioSys : public IManager
	{
		// singleton
	private:
		static AudioSys* instance;
		AudioSys() : sound_path(""), sound_dev(nullptr), sound_src(nullptr), cur_work_dir("") {};
	public:
		static AudioSys* Instance();
		~AudioSys() {};

	public:
		void init();
		int start();
		void update();
		int stop();
		void release();

		// Usage functions //
		void playSound(const std::string& name);
		void getWorkPath(const std::string& path);


	private:
		std::string cur_work_dir;
		std::string sound_path;
		SoundDevice* sound_dev;
		SoundSource* sound_src;
	};
}