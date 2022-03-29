#include "Renderer.h"
#include "Logger/LogModule.h"

//singleton
Renderer* Renderer::instance = nullptr;
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
Renderer* Renderer::Instance()
{
	if (nullptr == instance)
		return new Renderer();
	else
	{
		return instance;
	}
}

static void glfw_error_callback(int error, const char* description)
{
	//later change it into log system
	QERROR("Glfw Error %d: %s\n", error, description);
}


void Renderer::TestDrawDemo()
{
	//a lovely triangle
	/*const float triangles[] = {
	-0.5f,-0.5f,0.0f,
	0.5f,-0.5f,0.0f,
	0.0f,0.5f,0.0f,
	};

	//apply for VAO
	GLuint vertext_array_object;
	glGenVertexArrays(1, &vertext_array_object);
	glBindVertexArray(vertext_array_object);

	//apply for VBO
	GLuint vertext_buffer_object;
	glGenBuffers(1, &vertext_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertext_buffer_object);
	//bind
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//shader
	const char* vertex_shader_source =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos,1.0);\n"
		"}\n\0";
	const char* fragment_shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
		"}\n\0";

	//compile shader
	//create a vertx shader
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	//push
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	//compile
	glCompileShader(vertex_shader);

	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		QERROR("VERTEX SHADER COMPILE ERROR\n", info_log);
	}

	//fragment
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		QERROR ("FRAG SHADER COMPILE ERROR\n", info_log);
	}

	//link
	int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		QERROR("SHADER PROGRAM LINK ERROR\n", info_log);
	}

	//release
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	//QDEBUG("TestDrawDemo: ")

	render_context.shader_program = shader_program;
	render_context.VAO_handle = vertext_array_object;*/
}


/// <summary>
/// init
/// </summary>
int Renderer::init()
{
	/*according to the config
	init opengl correct version 
	or vulkan*/
	// Setup window
	//glfwSetErrorCallback(glfw_error_callback);
	//if (!glfwInit())
	//	return 1;

	// GL 3.0 + GLSL 130
	/*const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);*/

	//Create GLFW window with graphics context
	//context = glfwCreateWindow(width, height, title, NULL, NULL);

	//if (context == NULL)
	//	return 1;
	//glfwMakeContextCurrent(context);
	//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//assert(status, "failed to init");

	//for test something(draw something on the screen)
	//TestDrawDemo();

	//init_GUI(context);

	//glfwSwapInterval(1);
	return 0;
}

/// <summary>
/// start
/// </summary>
/// <returns>res</returns>
int Renderer::start()
{
	return 0;
}

/// <summary>
/// update
/// </summary>
int Renderer::render_loop()
{
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

	//give a clear color of the mainwindow
	glClearColor(0.45f, 0.55f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);

	TestDrawDemo();

	bindTexture();
	//loop for render scene
	//scene_loop();

	//loop for Editor UI
	//editor_loop(context);

	return 0;
}

int Renderer::stop()
{
	return 0;
}


const GLFWwindow* Renderer::get_window_context()
{
	return context;
}


void Renderer::scene_loop()
{
	//use shader
	glUseProgram(render_context.shader_program);

	//draw
	glBindVertexArray(render_context.VAO_handle);//bind VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);//draw tri
	glBindVertexArray(0);//unbind
}

void Renderer::bindTexture() {
	// generate texture
	glGenTextures(1, &textureFBO);
	glBindTexture(GL_TEXTURE_2D, textureFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFBO, 0);
}


unsigned int Renderer::returnFBO() {
	//QDEBUG("Returning texture: {0}", textureFBO);
	return textureFBO;
}
