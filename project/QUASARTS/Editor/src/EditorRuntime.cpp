#include "EditorRuntime.h"

EditorRuntime::EditorRuntime()
{

}
EditorRuntime::~EditorRuntime()
{

}

/*
	need to refer to a specific graphic context
	for example : in editor mode, editor runtime's context is the glfw window but game runtime's context is the scene view;
	later it will be implemented in render sys
*/

void EditorRuntime::init()
{
	_init_imgui();
}
void  EditorRuntime::on_update()
{
}
void  EditorRuntime::on_imgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	file_input();
	menubar();


	// Rendering
	ImGui::Render();
	int display_w, display_h;
	//glfwGetFramebufferSize(window, &display_w, &display_h);

	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void  EditorRuntime::on_quit()
{
}

#include "EditorUI.h"
#include "LogModule.h"
void EditorRuntime::_init_imgui()
{
	QTRACE("old version, need to change later");
	init_GUI(QEngine::appInst->get_window()->GetNativeWindow());
}

void  EditorRuntime::_file_input()
{
	file_input();
}

std::string  EditorRuntime::_OpenFileDialogue()
{
	OpenFileDialogue();
}

void  EditorRuntime::_menubar()
{
	menubar();
}
