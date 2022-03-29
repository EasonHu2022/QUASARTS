#pragma once
#include "Core/IModule.h"
#include "spdlog/spdlog.h"
#include <memory>
#include "Core/Core.h"


#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__,'\\')?(strrchr(__FILE__,'\\')+1):__FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__,'/')?(strrchr(__FILE__,'/')+1):__FILE__)
#endif 

#ifndef SUFFIX
#define SUFFIX(msg) std::string(msg).append("  <")\
                    .append(__FILENAME__).append("> <").append(__FUNCTION__)\
                    .append("> <").append(std::to_string(__LINE__))\
                    .append(">").c_str()
#endif 

//QUASARTS Trace 
//Trace Macro for Engine Trace
#define QTRACE(msg,...)		LogModule::Instance()->get_engine_logger()->trace(SUFFIX(msg),##__VA_ARGS__)

//Trace Macro for Game Trace
#define TRACE(msg,...)		LogModule::Instance()->get_game_logger()->trace(SUFFIX(msg),##__VA_ARGS__)

//Log Macro for Engine Degbug
#define QDEBUG(...)			LogModule::Instance()->get_engine_logger()->debug(__VA_ARGS__);

//Log Macro for Game Degbug
#define DEBUG(...)			LogModule::Instance()->get_game_logger()->debug(__VA_ARGS__);

//Log Macro for Engine Error
#define QERROR(msg,...)		LogModule::Instance()->get_engine_logger()->error(SUFFIX(msg),##__VA_ARGS__);

//Log Macro for Game Error
#define ERROR(msg,...)		LogModule::Instance()->get_game_logger()->error(SUFFIX(msg),##__VA_ARGS__);

//Log Macro for Engine Warn
#define QWARN(...)			LogModule::Instance()->get_engine_logger()->warn(__VA_ARGS__);

//Log Macro for Game Warn
#define WARN(...)			LogModule::Instance()->get_game_logger()->warn(__VA_ARGS__);


class  QS_API LogModule : public IModule
{

private:
	static LogModule* instance;
public:
	static LogModule* Instance();

private:
	static std::shared_ptr<spdlog::logger> q_EngineLogger;
	static std::shared_ptr<spdlog::logger> q_GameLogger;
public:
	inline static std::shared_ptr<spdlog::logger>& get_engine_logger() { return q_EngineLogger; };
	inline static std::shared_ptr<spdlog::logger>& get_game_logger() { return q_GameLogger; };

public:
	/// <summary>
	/// init the memoryModule
	/// </summary>
	void init() override;


	/// <summary>
	/// start the memoryModule
	/// </summary>
	/// <returns></returns>
	int start() override;

	/// <summary>
	/// update
	/// </summary>
	void update() override;

	/// <summary>
	/// stop the Memory module
	/// </summary>
	/// <returns></returns>
	int stop() override;

	/// <summary>
	/// release the memory module
	/// </summary>
	void release() override;
};
