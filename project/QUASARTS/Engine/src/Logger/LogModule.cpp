#include "LogModule.h"
#include <spdlog/sinks/stdout_color_sinks.h>
//singleton
LogModule* LogModule::instance = nullptr;
std::shared_ptr<spdlog::logger> LogModule::q_EngineLogger(nullptr);
std::shared_ptr<spdlog::logger> LogModule::q_GameLogger(nullptr);
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
LogModule* LogModule::Instance()
{
	if (nullptr == instance)
		return new LogModule();
	else
	{
		return instance;
	}
}

/// <summary>
/// init
/// </summary>
void LogModule::init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	q_EngineLogger = spdlog::stdout_color_mt("QENGINE");
	q_EngineLogger->set_level(spdlog::level::trace);

	q_GameLogger = spdlog::stdout_color_mt("GAME");
	q_GameLogger->set_level(spdlog::level::trace);
}

/// <summary>
/// start
/// </summary>
/// <returns>res</returns>
int LogModule::start()
{
	return 0;
}

/// <summary>
/// update
/// </summary>
void LogModule::update()
{
}

/// <summary>
/// stop
/// </summary>
/// <returns>res</returns>
int LogModule::stop()
{
	return 0;
}

/// <summary>
/// release
/// </summary>
void LogModule::release()
{
}