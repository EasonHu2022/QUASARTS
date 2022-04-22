#include "Application.h"
#include "Gui/GuiWrapper.h"
#include "Render/Renderer.h"
#include "Logger/LogModule.h"
#include "Scene/PhysicsSystem.h"
#include "Event/EventModule.h"
//#include "Scripts/ScriptsSystem.h"
#include "ECS/System/ScriptSystem.h"
#include "Audio/AudioSystem.h"
#include "Render/Renderer.h"
#include "ResourceManager/ResourceManager.h"

namespace Engine
{
	Application* Application::Instance = NULL;

	Application::Application()
	{
		Instance = this;
		bIs_Running = false;

		/******temp*********/
		entityWorld = new EntityWorld();
		miniecs = new miniECS();
		scene = new Scene();


		/*************************Create and Init********************************/
		LogModule::Instance()->init();
		EventModule::Instance()->init();
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(WindowClosed));
		ScriptSystem::Instance()->init();
		ResourceManager::Instance();
		AudioSys::Instance()->init();
		ECSManager::Instance()->init();
		ECSManager::Instance()->set_scene(scene);
		ResourceManager::Instance()->init();
		//create window for app
		m_window = Window::create(WindowProps(name));
		renderSystem = new RenderSystem();
		Renderer::Instance(); 
		/*************************Create and Init********************************/

	}
	Application:: ~Application()
	{
		//do release things
		GuiWrapper::release();
	}

	/// <summary>
	/// init
	/// </summary>
	void Application::init()
	{

		/***************later init things*************************/
		PhysicsSystem::Instance()->init();
		/*Renderer::Instance()->init();*/
		Renderer::Instance()->init();
		renderSystem->init();
		//do init things
		GuiWrapper::init();
		Input::init();
		/***************later init things*************************/


	}


	void Application::start()
	{

		bIs_Running = true;

		/***************later start things*************************/
		LogModule::Instance()->start();
		PhysicsSystem::Instance()->start();
		EventModule::Instance()->start();
		ScriptSystem::Instance()->start();

		//main loop
		while (bIs_Running)
		{

			/*
				render frame
			*/
			GuiWrapper::begin();
			{
				on_update();
				on_render();
			}
			GuiWrapper::end();

			if (bIs_Running)
				m_window->swap_buffer();
		}

		//on destroy
		on_release();
	}

	void Application::on_render()
	{
		/**************render update render frame***********************/
		Renderer::Instance()->render();
		/**************render update render frame***********************/
	}

	void Application::on_gui()
	{
		//generally do nothing
	}

	void Application::on_update()
	{
		/***************logic update logic frame************************/
		Input::reset_state();
		m_window->on_update();
		EventModule::Instance()->update();
		ScriptSystem::Instance()->update();
		PhysicsSystem::Instance()->update();
		renderSystem->update();
		AudioSys::Instance()->update();
		on_gui();
		/***************logic update logic frame************************/
	}


	void Application::close()
	{
		bIs_Running = false;
	}

	void Application::on_release()
	{
		/*********************release things**********************************/
		LogModule::Instance()->release();
		PhysicsSystem::Instance()->release();
		EventModule::Instance()->release();
		AudioSys::Instance()->release();
		renderSystem->release();
		Renderer::Instance()->release();
		/*********************release things**********************************/
	}

	void Application::EV_CALLBACK_SIGNATURE(WindowClosed)
	{
		QDEBUG("closed");
		close();
	}

}
