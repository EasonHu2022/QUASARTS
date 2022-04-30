#include "MeshRenderer.h"
#include "Logger/LogModule.h"
#include "Core/Application.h"
#include "glm/glm.hpp"

namespace Engine
{
	//singleton
	MeshRenderer* MeshRenderer::instance = nullptr;
	/// <summary>
	/// return an instance
	/// </summary>
	/// <returns></returns>
	MeshRenderer* MeshRenderer::Instance()
	{
		if (nullptr == instance)
			//instance = new MeshRenderer();

		return instance;

	}

	MeshRenderer::MeshRenderer(RenderContext *_renderContext) :  IRenderer(_renderContext)
	{
		
	}

	MeshRenderer::~MeshRenderer()
	{
	}


	

	/// <summary>
	/// init
	/// </summary>
	int MeshRenderer::init()
	{


		return 0;
	}

	/// <summary>
	/// update
	/// </summary>
	int MeshRenderer::render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderContext->frameBufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//give a clear color of the window
		glClearColor(0.45f, 0.55f, 0.6f, 1.0f);

		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

		renderContext->commit_camera_data();
		renderContext->commit_lighting_data();


		while (renderContext->renderQueue->get_size() != 0)
		{
			auto buffer = renderContext->renderQueue->get();
			//activate shader
			buffer->shader_program->use();
			//model : get from transform component
			glm::mat4 model = buffer->model;
			buffer->shader_program->setMat4("model", model);
			buffer->shader_program->setVec3("viewPos", renderContext->cameraContext->pos);
			//buffer->shader_program->setMat4("lightSpaceMatrix", renderContext->lightingContext->lights[0].lightSpaceMatrix);
			//buffer->shader_program->setInt("shadowMap", depthTextureArray);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, renderContext->depthTextureArray);

			glBindVertexArray(buffer->_VAO);
			glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			renderContext->renderQueue->pop();
			delete buffer;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		return 0;
	}

	int MeshRenderer::stop()
	{
		return 0;
	}


};
