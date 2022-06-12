#pragma once
#include "Core.h"
#include <vector>
#include "Window/Window.h"
#include "Gui/GuiWrapper.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Event/EventModule.h"
#include "Core/EntityWorld.h"
#include "Core/miniECS.h"
#include "ECS/ECSManager.h"
#include "Render/RenderSystem.h"
#include "Render/RenderContext.h"
#include "Render/MeshRenderer.h"
#include "Render/PostProcessing.h"
#include "Render/ShadowRenderer.h"
#include "Render/SkyboxRenderer.h"
#include "ECS/System/ParticleSystem.h"
#include "Core/Input.h"

namespace Engine
{

	class QS_API Application
	{
	private:
		std::string name = "Orbit Editor V1.0";
		Window* m_window = NULL;
		bool bIs_Running;

	public:
		static Application* Instance;
		EntityWorld* entityWorld;
		miniECS* miniecs;
		Scene* scene;
		RenderSystem* renderSystem;
		
		/*******render*********/
		RenderContext* renderContext;
		MeshRenderer* meshRenderer;
		ShadowRenderer* shadowRenderer;
		SkyBoxRenderer* skyboxRenderer;
		PostProcessing* postProcessing;
		ParticleMaster particleMaster;
		ParticleSystem* particleSystem;

	public:
		Window* get_window() { return m_window; }
		GLFWwindow* get_glfw_context() { return m_window->get_native_window(); }
		Application();
		virtual ~Application();
		virtual void init();
		virtual void start();
		virtual void close();
		virtual void on_gui();
		virtual void on_update();
		virtual void on_render();
		virtual void on_release();

		void EV_CALLBACK_SIGNATURE(WindowClosed);
	};
	Application* create_application();
}


