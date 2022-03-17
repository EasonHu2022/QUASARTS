#include "QEngine.h"
#include "MemoryModule.h"
#include "LogModule.h"
#include "PhysicsManager.h"
#include "FileModule.h"

namespace Engine 
{
	
	
	Application* QEngine::appInst = NULL;
	/// <summary>
	/// initialize the application
	/// </summary>
	void QEngine::init(Application* app)
	{


		//init the log module
		LogModule* log = LogModule::Instance();
		log->init();
		modules.push_back(log);

		//init the File module
		FileModule* file = FileModule::Instance();
		file->init();
		modules.push_back(file);

		//init the memory module as the very first module
		MemoryModule* mem = MemoryModule::Instance();
		mem->init();
		modules.push_back(mem);

		//test log
		//QTRACE("Test for QTRACE");
		//TRACE("Test for TRACE");
		//QDEBUG("Test for QDEBUG");
		//DEBUG("Test for DEBUG");
		//QERROR("Test for QERROR");
		//ERROR("Test for ERROR");
		//QWARN("Test for QWARN");
		//WARN("Test for WARN");



		////init event manager
		//EventManager* eve = EventManager::Instance();
		//eve->init();
		//managers.push_back(eve);

		//init physics manager
		PhysicsManager* phys = PhysicsManager::Instance();
		phys->init();
		managers.push_back(phys);

		/// <summary>
		/// create an run time application
		/// </summary>
		appInst = app;
	}

	/// <summary>
	/// start the application
	/// </summary>
	void QEngine::start()
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

		/// <summary>
		/// start the runtime app
		/// </summary>
		appInst->start();



		//the main loop
		while (true)
		{
			/// <summary>
			/// push update to application
			/// </summary>
			appInst->update();


			//the order of each loop is very important
			for (auto manager : managers)
			{
				manager->update();
			}

			//they are not involved in render frame
			for (auto module : modules)
			{
				if (module->NeedUpdate)
				{
					module->update();
				}
			}
		}



		//call all stop() functions

		appInst->stop();

		for (auto manager : managers)
		{
			manager->stop();
		}
		for (auto module : modules)
		{
			module->stop();
		}

		stop();

	}

	/// <summary>
	/// stop the application
	/// </summary>
	void QEngine::stop()
	{
		for (auto module : modules)
		{

			module->release();

		}
		/*renderer->release();*/

	}

}
