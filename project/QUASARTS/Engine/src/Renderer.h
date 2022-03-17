//#pragma once
//#include "glfw3.h"
//#include "glad.h"
//
//
//
//struct RenderContext
//{
//	int shader_program;
//	GLuint VAO_handle;
//};
//
//class  Renderer 
//{
//
//
//
//private:
//	static Renderer* instance;
//public:
//	static Renderer* Instance();
//public:
//	const GLFWwindow* get_window_context();
//private:
//	GLFWwindow* context;
//	RenderContext render_context;
//	int width = 1280;
//	int height = 720;
//
//	const char* title = "QUASARTS Editor";
//
//
//public:
//	/// <summary>
//	/// init the memoryModule
//	/// </summary>
//	int init();
//
//
//	/// <summary>
//	/// start the memoryModule
//	/// </summary>
//	/// <returns></returns>
//	int start();
//
//	/// <summary>
//	/// update
//	/// </summary>
//	int render_loop();
//
//	/// <summary>
//	/// stop the Memory module
//	/// </summary>
//	/// <returns></returns>
//	int stop();
//
//	/// <summary>
//	/// release the memory module
//	/// </summary>
//	void release();
//
//private:
//	/// <summary>
//	/// the main render loop for scene render
//	/// </summary>
//	void scene_loop();
//
//
//	void editor_loop(GLFWwindow*);
//
//	void TestDrawDemo();
//};
