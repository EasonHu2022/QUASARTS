#include "MemoryModule.h"
//singleton
MemoryModule* MemoryModule::instance = nullptr;

/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
MemoryModule* MemoryModule::Instance()
{
	if (nullptr == instance)
		return new MemoryModule();
	else
	{
		return instance;
	}
}

/// <summary>
/// init
/// </summary>
void MemoryModule::init()
{
}

/// <summary>
/// start
/// </summary>
/// <returns>res</returns>
int MemoryModule::start()
{
	return 0;
}

/// <summary>
/// update
/// </summary>
void MemoryModule::update()
{
}

/// <summary>
/// stop
/// </summary>
/// <returns>res</returns>
int MemoryModule::stop()
{
	return 0;
}

/// <summary>
/// release
/// </summary>
void MemoryModule::release()
{
}