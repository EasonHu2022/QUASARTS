#pragma once
#include "glfw3.h"
#include "glad.h"
#include "Core/Core.h"
#include "Render/RenderQueue.h"
//#include "GL.h"



struct RenderContext
{
	int shader_program;
	GLuint VAO_handle;
};

class QS_API RendererTemp
{



private:
	static RendererTemp* instance;
public:
	static RendererTemp* Instance();
public:
	unsigned int tbo;
	unsigned int fbo;
private:
	RenderContext render_context;
public:
	RenderQueue* renderQueue;

public:
	/// <summary>
	/// init the memoryModule
	/// </summary>
	//int init(int width, int height, char* title);
	int init();

	/// <summary>
	/// update
	/// </summary>
	int render();

	/// <summary>
	/// stop the Memory module
	/// </summary>
	/// <returns></returns>
	int stop();

	/// <summary>
	/// release the memory module
	/// </summary>
	//void release();

	unsigned int get_rt();


private:

	void Shader();
};
