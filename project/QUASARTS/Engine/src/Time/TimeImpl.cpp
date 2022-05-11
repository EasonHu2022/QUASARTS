#include "TimeImpl.h"

using namespace std::chrono;


namespace Engine
{
	TimeImpl::TimeImpl()
	{

	}

	TimeImpl::~TimeImpl()
	{

	}


	// IModule overrides //

	void TimeImpl::init()
	{
		initTime = steady_clock::now();

	} // init()

	int TimeImpl::start()
	{
		oldFrameTime = steady_clock::now();

		return 0;

	} // start()

	void TimeImpl::update()
	{
		newFrameTime = steady_clock::now();
		deltaFrameTime = newFrameTime - oldFrameTime;

		// Cap dT.
		if (deltaFrameTime.count() > Q_MAX_DELTA_T)
			deltaFrameTime = duration< double >(Q_MAX_DELTA_T);

		oldFrameTime = newFrameTime;

	} // update()

	int TimeImpl::stop()
	{
		return 0;

	} // stop()

	void TimeImpl::release()
	{

	} // release()


	// Usage //

	QTime TimeImpl::getDeltaFrameTime()
	{
		return QTime(deltaFrameTime.count());

	} // getDeltaFrameTime()


	QTime TimeImpl::getTimeSinceInit()
	{
		return QTime(duration<double>(steady_clock::now() - initTime).count());

	} // getTimeSinceInit()

	

}