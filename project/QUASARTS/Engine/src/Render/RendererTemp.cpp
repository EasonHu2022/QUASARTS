#include "RendererTemp.h"
#include "Logger/LogModule.h"

//singleton
RendererTemp* RendererTemp::instance = nullptr;
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
RendererTemp* RendererTemp::Instance()
{
	if (nullptr == instance)
		instance =  new RendererTemp();

		return instance;
	
}




void RendererTemp::TestDrawDemo()
{
	//a lovely triangle
	const float triangles[] = {
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

	QDEBUG("TestDrawDemo: ")

	render_context.shader_program = shader_program;
	render_context.VAO_handle = vertext_array_object;
}


/// <summary>
/// init
/// </summary>
int RendererTemp::init()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &tbo);
	glBindTexture(GL_TEXTURE_2D, tbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//TestDrawDemo();
	
	return 0;
}


/// <summary>
/// update
/// </summary>
int RendererTemp::render()
{
	




	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	
	//glEnable(GL_DEPTH_TEST);
	//give a clear color of the window
	glClearColor(0.45f, 0.55f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 400, 400);
	while (renderQueue->get_size() != 0)
	{
		auto buffer = renderQueue->get();

		glUseProgram(render_context.shader_program);
		glBindVertexArray(buffer);//bind VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);//draw tri
		glBindVertexArray(0);//unbind
		renderQueue->pop();
	}
	
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return 0;
}

int RendererTemp::stop()
{
	return 0;
}


void RendererTemp::bind_texture() {

	
}



unsigned int RendererTemp::get_rt() {
	//QDEBUG("Returning texture: {0}", textureFBO);
	return tbo;
}
