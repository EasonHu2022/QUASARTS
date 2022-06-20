#include "Application.h"
#include "Gui/GuiWrapper.h"
#include "Logger/LogModule.h"
#include "Scene/PhysicsSystem.h"
#include "Event/EventModule.h"
#include "ECS/System/ScriptSystem.h"
#include "ECS/System/AudioSystem.h"
#include "ResourceManager/ResourceManager.h"
#include "Time/TimeModule.h"
#include "ECS/System/CollisionSystem.h"
#include "ECS/System/OrbitSystem.h"
#include "ECS/System/CombatSystem.h"
#include "ECS/System/CameraSystem.h"
#include "ECS/System/UISystem.h"

#include "ECS/Showcase-Video.h"

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
		ECSManager::Instance()->register_system(SYSTEM_AUDIO, AudioSystem::Instance());
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
		postProcessing = new PostProcessing(renderContext);
		CollisionSystem* collisionSystem = CollisionSystem::Instance();
		ECSManager::Instance()->register_system(SYSTEM_COLLISION, collisionSystem);
		OrbitSystem* orbitSystem = OrbitSystem::Instance();
		ECSManager::Instance()->register_system(SYSTEM_ORBIT, orbitSystem);
		particleSystem = new ParticleSystem();
		ECSManager::Instance()->register_system(SYSTEM_PARTICLE, particleSystem);
		CombatSystem::Instance()->init();
		ECSManager::Instance()->register_system(SYSTEM_COMBAT, CombatSystem::Instance());
		CameraSystem::Instance()->init();
		ECSManager::Instance()->register_system(SYSTEM_CAMERA, CameraSystem::Instance());
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
		postProcessing->init();
		ParticleMaster::Instance()->init(renderContext);
		//particleMaster.init(renderContext);
		renderSystem->init();
		//do init things
		GuiWrapper::init();
		Input::init();
		ECSManager::Instance()->register_system(SYSTEM_RENDER, renderSystem);
		CollisionSystem::Instance()->init();
		OrbitSystem::Instance()->init();
		UISystem::Instance()->init(renderContext);
		ECSManager::Instance()->register_system(SYSTEM_UI, UISystem::Instance());
		/***************later init things*************************/


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
		OrbitSystem::Instance()->start();		// TODO : start when game starts, not in editor
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

		//do all of the postProcessing before final render
		postProcessing->gaussianBlur();

		skyboxRenderer->render();
		postProcessing->render();

		// Render particles last:
		ParticleMaster::Instance()->render();

		// Render UI super last:
		UISystem::Instance()->render();
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
		
		
		renderSystem->update();
		AudioSystem::Instance()->update();
		on_gui();
		ParticleMaster::Instance()->update();
		particleSystem->update();
		CollisionSystem::Instance()->update();
		OrbitSystem::Instance()->update();
		CombatSystem::Instance()->update();
		CameraSystem::Instance()->update();

		ShowcaseVideo::Instance()->update();
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
		postProcessing->release();
		//particleMaster.release();
		/*********************release things**********************************/
	}

	void Application::EV_CALLBACK_SIGNATURE(WindowClosed)
	{
		QDEBUG("closed");
		close();
	}

}
