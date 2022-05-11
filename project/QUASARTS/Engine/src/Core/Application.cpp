#include "Application.h"
#include "Gui/GuiWrapper.h"
#include "Logger/LogModule.h"
#include "Scene/PhysicsSystem.h"
#include "Event/EventModule.h"
#include "ECS/System/ScriptSystem.h"
#include "ECS/System/AudioSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "Time/TimeModule.h"

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
		/******temp*********/

		/*************************Create and Init********************************/
		
		TimeModule::Instance()->init();
		LogModule::Instance()->init();
		EventModule::Instance()->init();
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(WindowClosed));
		ScriptSystem::Instance()->init();
		ECSManager::Instance()->register_system(SYSTEM_SCRIPT, ScriptSystem::Instance());
		ResourceManager::Instance();
		AudioSystem::Instance()->init();
		ECSManager::Instance()->init();
		ResourceManager::Instance()->init();
		//create window for app
		m_window = Window::create(WindowProps(name));
		renderSystem = new RenderSystem();
		ECSManager::Instance()->register_system(SYSTEM_RENDER, renderSystem);
		renderContext = new RenderContext();
		shadowRenderer = new ShadowRenderer(renderContext);
		meshRenderer = new MeshRenderer(renderContext);
		skyboxRenderer = new SkyBoxRenderer(renderContext);
		collisionSystem = new CollisionSystem();
		ECSManager::Instance()->register_system(SYSTEM_COLLISION, collisionSystem);
		orbitSystem = new OrbitSystem();
		ECSManager::Instance()->register_system(SYSTEM_ORBIT, orbitSystem);
		particleSystem = new ParticleSystem(90.0f, 3.0f, -10.0f, 2.0f, 0.1f, 0.3f, 0.2f, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);
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
		renderContext->init();
		shadowRenderer->init();
		meshRenderer->init();
		skyboxRenderer->init();
		ParticleMaster::Instance()->init(renderContext);
		//particleMaster.init(renderContext);
		renderSystem->init();
		//do init things
		GuiWrapper::init();
		Input::init();
		ECSManager::Instance()->register_system(SYSTEM_RENDER, renderSystem);
		/***************later init things*************************/
		collisionSystem->init();
		orbitSystem->init();


	}


	void Application::start()
	{

		bIs_Running = true;

		/***************later start things*************************/
		TimeModule::Instance()->start();
		LogModule::Instance()->start();
		PhysicsSystem::Instance()->start();
		EventModule::Instance()->start();
		ScriptSystem::Instance()->start();
		//orbitSystem->start();		// TODO : start when game starts, not in editor
		/***************later start things*************************/

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
		shadowRenderer->render();

		meshRenderer->render();

		skyboxRenderer->render();

		ParticleMaster::Instance()->render();
		//particleMaster.render();
		/**************render update render frame***********************/
	}

	void Application::on_gui()
	{
		//generally do nothing
	}

	void Application::on_update()
	{
		/***************logic update logic frame************************/
		TimeModule::Instance()->update();
		Input::reset_state();
		m_window->on_update();
		EventModule::Instance()->update();
		ScriptSystem::Instance()->update();
		PhysicsSystem::Instance()->update();
		particleSystem->generateParticle(glm::vec3(0.0f, 0.0f, -1.0f));
		ParticleMaster::Instance()->update();
		renderSystem->update();
		AudioSystem::Instance()->update();
		on_gui();
		collisionSystem->update();
		/***************logic update logic frame************************/
	}


	void Application::close()
	{
		bIs_Running = false;
	}

	void Application::on_release()
	{
		/*********************release things**********************************/
		TimeModule::Instance()->release();
		LogModule::Instance()->release();
		PhysicsSystem::Instance()->release();
		EventModule::Instance()->release();
		AudioSystem::Instance()->release();
		renderSystem->release();
		meshRenderer->release();
		shadowRenderer->release();
		skyboxRenderer->release();
		//particleMaster.release();
		/*********************release things**********************************/
	}

	void Application::EV_CALLBACK_SIGNATURE(WindowClosed)
	{
		QDEBUG("closed");
		close();
	}

}
