#include "TimeModule.h"
#include "TimeImpl.h"


namespace Engine
{
	TimeModule* TimeModule::instance = nullptr;
	TimeImpl* TimeModule::implInstance = nullptr;
	TimeModule* TimeModule::Instance()
	{
		if (!instance)
		{
			instance = new TimeModule();
			implInstance = new TimeImpl();
		}
		return instance;
	}


	// IModule overrides //

	void TimeModule::init() {
		implInstance->init();
	} // init()

	int TimeModule::start() {
		return implInstance->start();
	} // start()

	void TimeModule::update() {
		implInstance->update();
	} // update()

	int TimeModule::stop() {
		return implInstance->stop();
	} // stop()

	void TimeModule::release() {
		implInstance->release();
	} // release()


	// Public Usage //

	QTime TimeModule::get_frame_delta_time()
	{
		return implInstance->getDeltaFrameTime();
	}

	QTime TimeModule::get_time()
	{
		return implInstance->getTimeSinceInit();
	}
}