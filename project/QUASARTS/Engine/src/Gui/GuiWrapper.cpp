#include "GuiWrapper.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "Logger/LogModule.h"

namespace Engine
{

    void GuiWrapper::init()
    {
        /*
            here need to be more abstract if we need diff gui
            now just for imgui
        */

        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;


        /*io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;*/

        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        //get window from app
        auto glfwWindowContext = Engine::Application::Instance->get_glfw_context();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(glfwWindowContext, true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }




    void GuiWrapper::release()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiWrapper::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void GuiWrapper::end()
    {
        //commit and render

        ImGuiIO& io = ImGui::GetIO();
        auto app = Engine::Application::Instance;
        io.DisplaySize = ImVec2((float)app->get_window()->get_width(), (float)app->get_window()->get_height());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


}

