#include "Application.h"
#include "Gui/GuiWrapper.h"
#include "Render/Renderer.h"
#include "Logger/LogModule.h"
#include "Scene/PhysicsSystem.h"
#include "Event/EventModule.h"
#include "Core/Input.h"
#include "Scripts/ScriptsSystem.h"
#include "Render/RendererTemp.h"

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

		LogModule::Instance()->init();

		EventModule::Instance()->init();

		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(WindowClosed));

		ScriptsSys::Instance()->init();

		loaderFactory = new MeshLoaderFactory();
		//create window for app
		m_window = Window::create(WindowProps(name));		

		RendererTemp::Instance()->init();


		ECSManager::Instance()->init();				 // scene test
		scene = new Scene();						 // scene test
		ECSManager::Instance()->set_scene(scene);	 // scene test
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
		

		/*
			later change to ECS mode
			later remove instance
		*/
		
		PhysicsSystem::Instance()->init();
		
		/*Renderer::Instance()->init();*/

		//do init things
		GuiWrapper::init();

		Input::init();

	}


	void Application::start()
	{

		bIs_Running = true;

		/*
			temp
		*/
		LogModule::Instance()->start();
		PhysicsSystem::Instance()->start();
		EventModule::Instance()->start();
		ScriptsSys::Instance()->start();

		


		/// <summary>
		/// for test
		/// </summary>
		

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

			/*
				System Manager Update
			*/
			if(bIs_Running)
				m_window->swap_buffer();
		}

		//on destroy
		on_release();
	}

	void Application::on_render()
	{
		//Renderer::Instance()->render_loop();
		//Renderer::Instance()->render_loop();
		RendererTemp::Instance()->render();
	}

	void Application::on_gui()
	{
		//generally do nothing
	}

	void Application::on_update()
	{
		//reset input
		Input::reset_state();
		m_window->on_update();

		EventModule::Instance()->update();


		LogModule::Instance()->update();
		PhysicsSystem::Instance()->update();
		
		on_gui();
	}


	void Application::close()
	{
		bIs_Running = false;
	}

	void Application::on_release()
	{
		//release everything
		LogModule::Instance()->release();
		PhysicsSystem::Instance()->release();
		EventModule::Instance()->release();
	}

	void Application::EV_CALLBACK_SIGNATURE(WindowClosed)
	{
		QDEBUG("closed");
		close();
	}

}
