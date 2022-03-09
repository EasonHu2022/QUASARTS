#include "Application.h"
#include "MemoryModule.h"
#include "LogModule.h"
#include "PhysicsManager.h"

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

	

	//init the memory module as the very first module
	MemoryModule* mem = MemoryModule::Instance();
	mem->init();
	modules.push_back(mem);

	//init window
	renderer = new Renderer();
	renderer->init();

	
	//test log
	QTRACE("Test for QTRACE");
	TRACE("Test for TRACE");
	QDEBUG("Test for QDEBUG");
	DEBUG("Test for DEBUG");
	QERROR("Test for QERROR");
	ERROR("Test for ERROR");
	QWARN("Test for QWARN");
	WARN("Test for WARN");
	


	//init physics manager
	PhysicsManager* phys = PhysicsManager::Instance();
	phys->init();
	managers.push_back(phys);

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
			QERROR("start error fatal!!!");
			return;
		}
	}

	//call manager start in a certain order
	for (auto manager : managers)
	{
		int res = manager->start();
		if (res != 0)
		{
			//log the error by res 
			QERROR("start error fatal!!!");
			return;
		}
	}



	//the engine runtime loop
	while (true)
	{
		//the order of each loop is very important
		for (auto manager : managers)
		{
			manager->update();
		}



		//render loop
		if (renderer->render_loop() == 1)
		{
			break;
		}		



		//if we have post process, put it here




		//they are not involved in render frame
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
	renderer->release();

}