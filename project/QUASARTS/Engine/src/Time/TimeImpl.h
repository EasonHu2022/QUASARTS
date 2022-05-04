#pragma once

#include "Core/Core.h"
#include "Core/IModule.h"
#include "QTime.h"

#include <chrono>


namespace Engine
{
	class QS_API Time
	{

	};

	class QS_API TimeImpl : public IModule
	{
	public:
		TimeImpl();
		~TimeImpl();

		void init() override;
		int start() override;
		void update() override;
		int stop() override;
		void release() override;


		// Usage //
	public:
		QTime getDeltaFrameTime();
		QTime getTimeSinceInit();


		// Timekeeping //
	private:
		std::chrono::time_point< std::chrono::steady_clock >
			initTime,
			oldFrameTime, newFrameTime;

		std::chrono::duration< double >
			deltaFrameTime;
	};
}