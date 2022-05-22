#include "MeshRenderer.h"
#include "Logger/LogModule.h"
#include "Core/Application.h"
#include "glm/glm.hpp"

namespace Engine
{

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
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

		renderContext->commit_camera_data();
		renderContext->commit_lighting_data();
		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);
		

		while (renderContext->renderQueue->get_size() != 0)
		{
			auto buffer = renderContext->renderQueue->get();
			if (buffer->shader_program == NULL)
			{
				renderContext->renderQueue->pop();
				continue;
			}
				
			//activate shader
			buffer->shader_program->use();
			//model : get from transform component
			glm::mat4 model = buffer->model;
			glm::mat4 normalM = buffer->normalM;
			buffer->shader_program->setMat4("model", model);
			buffer->shader_program->setMat4("normalM", normalM);
			buffer->shader_program->setVec3("viewPos", renderContext->cameraContext->pos);
			buffer->shader_program->setVec3("ka",buffer->ka);
			buffer->shader_program->setVec3("kd", buffer->kd);
			buffer->shader_program->setVec3("ks", buffer->ks);
			buffer->shader_program->setVec3("ke", buffer->ke);
			buffer->shader_program->setFloat("emRange", buffer->emissiveRange);
			buffer->shader_program->setFloat("shininess", buffer->shininess);
			//buffer->shader_program->setMat4("lightSpaceMatrix", renderContext->lightingContext->lights[0].lightSpaceMatrix);
			//buffer->shader_program->setInt("shadowMap", depthTextureArray);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, renderContext->depthTextureArray);



			if (buffer->texture2d != NULL)
			{				
				buffer->texture2d->use(GL_TEXTURE1);
				GLint iTextureUniform = glGetUniformLocation(buffer->shader_program->ID, "colorTexure");
				glUniform1i(iTextureUniform, 1);
			}

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
