#pragma once
#include "IModule.h"
#include "IManager.h"
#include "Application.h"
#include <vector>
#include <iostream>
#include "Renderer.h"
namespace Engine
{
	
	class QEngine
	{
	public:
		static Application* appInst;

	public:
		QEngine() {};
		~QEngine() {};
	public:

		/// <summary>
		/// initialize 
		/// </summary>
		void init(Application* app);

		/// <summary>
		/// start the application
		/// </summary>
		void start();


		/// <summary>
		/// stop the application
		/// </summary>
		void stop();


	public:
		//hold the pointer of 3 very basic module
		std::vector <IModule*> modules;
		//hold managers
		std::vector <IManager*> managers;

	};
	Application* create_application();
}

