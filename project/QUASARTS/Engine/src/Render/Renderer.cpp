#include "Renderer.h"
#include "Logger/LogModule.h"
#include "Core/Application.h"
#include "glm/glm.hpp"

namespace Engine
{
	//singleton
	Renderer* Renderer::instance = nullptr;
	/// <summary>
	/// return an instance
	/// </summary>
	/// <returns></returns>
	Renderer* Renderer::Instance()
	{
		if (nullptr == instance)
			instance = new Renderer();

		return instance;

	}

	Renderer::Renderer()
	{
		renderQueue = new RenderQueue();
		cameraContext = new CameraContext();
		lightingContext = new LightingContext();

		//initialize databuffers
		matricesBuffer = new UniformBufferObject(UniformBlockIndex::matrices, 2 * sizeof(glm::mat4));
		//carefully allocate memory for light buffer
		lightBuffer = new UniformBufferObject(UniformBlockIndex::light, sizeof(LightingContext));

		//create shadow mapping shader
		shadow_mapping_shader = new Shader(vshPath.data(), fshPath.data());
	}

	Renderer::~Renderer()
	{
	}


	

	/// <summary>
	/// init
	/// </summary>
	int Renderer::init()                                              
	{

		matricesBuffer->init();
		lightBuffer->init();

		/*****************RenderTexture************************/
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &tbo);
		glBindTexture(GL_TEXTURE_2D, tbo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RT_WIDTH, RT_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RT_WIDTH, RT_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/*****************RenderTexture************************/

		/*****************ShadowMapping************************/
		//for many lights
		glEnable(GL_TEXTURE_2D_ARRAY);
		glGenTextures(1, &depthTextureArray);
		glBindTexture(GL_TEXTURE_2D_ARRAY, depthTextureArray);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, MAX_LIGHTING_COUNT,0, GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
		
		
		
		
		for (int i = 0; i < MAX_LIGHTING_COUNT; i++)
		{
			glGenFramebuffers(1, &depthMapFBO[i]);
			
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureArray, 0,i);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}		


		//temp for single light
	/*	glGenTextures(1, &depthTextureArray);
		glBindTexture(GL_TEXTURE_2D, depthTextureArray);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &depthMapFBO[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[0]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureArray, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

		/*****************ShadowMapping************************/

		


		return 0;
	}

	
	void Renderer::render_shadow_map()
	{		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);		
		glEnable(GL_DEPTH_TEST);
		//ConfigureShaderAndMatrices();
		auto lightCount = lightingContext->countLight;
		auto lightingdatas = lightingContext->lights;
		for (int i = 0; i < lightCount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
			glClear(GL_DEPTH_BUFFER_BIT);
			auto size = renderQueue->get_size();
			while (size > 0)
			{
				auto lightData = lightingdatas[i];
				auto buffer = renderQueue->get();
				//activate shader
				shadow_mapping_shader->use();

				glm::mat4 model = buffer->model;
				shadow_mapping_shader->setMat4("model", model);
				shadow_mapping_shader->setMat4("lightSpaceMatrix", lightData.lightSpaceMatrix);

				//optimation: combine all things into a vao then we just need to render once(because the shader is the same)
				glBindVertexArray(buffer->_VAO);
				glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				renderQueue->pop();
				//push for next render
				renderQueue->push(buffer);
				size--;
			}
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}	
		//temp
		/*if(renderQueue->get_size()>0)
			renderQueue->pop();*/
	}

	void Renderer::render_texture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//give a clear color of the window
		glClearColor(0.45f, 0.55f, 0.6f, 1.0f);
		
		glViewport(0, 0, RT_WIDTH, RT_HEIGHT);

		matricesBuffer->set_data(0, sizeof(glm::mat4), cameraContext->get_projection_data());
		matricesBuffer->set_data(sizeof(glm::mat4), sizeof(glm::mat4), cameraContext->get_view_data());
		lightBuffer->set_data(0, sizeof(LightingContext), lightingContext);


		while (renderQueue->get_size() != 0)
		{
			auto buffer = renderQueue->get();
			//activate shader
			buffer->shader_program->use();
			//model : get from transform component
			glm::mat4 model = buffer->model;
			buffer->shader_program->setMat4("model", model);
			buffer->shader_program->setVec3("viewPos", cameraContext->pos);
			buffer->shader_program->setMat4("lightSpaceMatrix", lightingContext->lights[0].lightSpaceMatrix);
			//buffer->shader_program->setInt("shadowMap", depthTextureArray);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, depthTextureArray);

			glBindVertexArray(buffer->_VAO);
			glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			renderQueue->pop();
			delete buffer;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	/// <summary>
	/// update
	/// </summary>
	int Renderer::render()
	{
		//render the shadow map
		render_shadow_map();

		//render to the render texture
		render_texture();

		
		return 0;
	}

	int Renderer::stop()
	{
		return 0;
	}








	unsigned int Renderer::get_renderTexture() {
		//QDEBUG("Returning texture: {0}", textureFBO);
		return tbo;
		
	}

};
