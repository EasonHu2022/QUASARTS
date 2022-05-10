#pragma once

#include "Core/Core.h"
#include "Core/IModule.h"
#include "Logger/LogModule.h"
#include "QTime.h"


namespace Engine
{
	class QS_API TimeImpl;

	class QS_API TimeModule : public IModule
	{
	public:
		static TimeModule* Instance();
	private:
		static TimeModule* instance;
		static TimeImpl* implInstance;
		TimeModule() {};
		~TimeModule() {};


		// IModule overrides //
	public:
		void init() override;
		int start() override;
		void update() override;
		int stop() override;
		void release() override;


		// Public Usage //
	public:
		/// <summary>
		/// Gets time in seconds between the current and previous frames.
		/// </summary>
		/// <returns>Framewise delta time</returns>
		QTime get_frame_delta_time();

		/// <summary>
		/// Gets time in seconds since engine initialisation.
		/// Value is computed during the call, and so is independent of frame times.
		/// </summary>
		/// <returns>Current engine time</returns>
		QTime get_time();
		

	};
}