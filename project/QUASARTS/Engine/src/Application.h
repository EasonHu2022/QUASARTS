#pragma once
#include "IModule.h"
#include "IManager.h"

#include <vector>
#include <iostream>
#include "Renderer.h"


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
	//hold managers
	std::vector <IManager*> managers;
	//ptr of renderer
	Renderer* renderer = nullptr;

};
