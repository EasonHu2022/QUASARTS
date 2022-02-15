#include "Application.h"
/// <summary>
/// entrance of the engine
/// put it in the engine layer
/// </summary>
/// <returns></returns>
int WinMain()
{
	//Create a singleton of the application
	Application* appInst = Application::Instance();

	//initialize the application
	appInst->init_Application();

	//start the application
	appInst->start_Application();

	return 0;
}