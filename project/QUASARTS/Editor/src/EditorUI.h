#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "iostream"
#include "glfw3.h"

// Decide OS
#if defined(_WIN32)
#include <windows.h>
#endif

void init_GUI(GLFWwindow*);
void file_input();
void menubar();
std::string OpenFileDialogue();