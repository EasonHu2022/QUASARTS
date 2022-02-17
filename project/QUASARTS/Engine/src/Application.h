#pragma once
#include "IModule.h"

#include <vector>
#include <iostream>
#include "MainWindow.h"


class Application
{
private:
	static Application* instance;
private:
	Application() {};
	~Application() {};
public:

	/// <summary>
	/// return an instance of the application
	/// </summary>
	/// <returns></returns>
	static Application* Instance();

	/// <summary>
	/// initialize the application
	/// </summary>
	void init_Application();

	/// <summary>
	/// start the application
	/// </summary>
	void start_Application();


	/// <summary>
	/// stop the application
	/// </summary>
	void stop_Application();


public:
	//hold the pointer of 3 very basic module
	std::vector <IModule*> modules;
	//ptr of mainwindow
	MainWindow* main_window = nullptr;

};
