#include "Application.h"
#include "MemoryModule.h"
#include "LogModule.h"

//singleton
Application* Application::instance = nullptr;
/// <summary>
/// Create a singleton of the application
/// </summary>
/// <returns>The unique instance of the application</returns>
Application* Application::Instance()
{
	if (nullptr == instance)
		return new Application();
	else
		return instance;
}

/// <summary>
/// initialize the application
/// </summary>
void Application::init_Application()
{
	//init the log module
	LogModule* log = LogModule::Instance();
	log->init();
	modules.push_back(log);

	//init window
	main_window = new MainWindow();
	main_window->Init();
	//init the memory module as the very first module
	MemoryModule* mem = MemoryModule::Instance();
	mem->init();
	modules.push_back(mem);
	
	//test log
	QTRACE("Test for QTRACE");
	TRACE("Test for TRACE");
	QDEBUG("Test for QDEBUG");
	DEBUG("Test for DEBUG");
	QERROR("Test for QERROR");
	ERROR("Test for ERROR");
	QWARN("Test for QWARN");
	WARN("Test for WARN");
	
}

/// <summary>
/// start the application
/// </summary>
void Application::start_Application()
{
	//call module start in a certain order
	for (auto module : modules)
	{
		int res = module->start();
		if (res != 0)
		{
			//log the error by res 
			std::cout << "start error fatal!!" << std::endl;
			return;
		}
	}



	//the engine runtime loop
	while (true)
	{
		//the order of each loop is very important
		//now I just put window and ui loop on the first
		if (!main_window->Update())
			break;

		for (auto module : modules)
		{
			if (module->NeedUpdate)
			{
				module->update();
			}
		}
	}

	stop_Application();

}

/// <summary>
/// stop the application
/// </summary>
void Application::stop_Application()
{
	for (auto module : modules)
	{

		module->release();

	}
	main_window->Release();

}