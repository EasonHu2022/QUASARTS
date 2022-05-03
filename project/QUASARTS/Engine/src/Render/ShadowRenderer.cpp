#include "ShadowRenderer.h"
#include "ResourceManager/FileModule.h"

namespace Engine
{
	ShadowRenderer::ShadowRenderer(RenderContext* renderContext) : IRenderer(renderContext)
	{
		//get default engine assets path
		auto path = FileModule::Instance()->get_internal_assets_path();
		//shadow mapping vertex shader path
		vshPath = path + "Shader\\ShadowMapping.vsh";
		//shadow mapping fragment shader path
		fshPath = path + "Shader\\ShadowMapping.fsh";
	}
	ShadowRenderer::~ShadowRenderer()
	{
	}
	int ShadowRenderer::init()
	{
		// create shadow mapping shader
		shadow_mapping_shader = new Shader(vshPath.data(), fshPath.data());
		
		return 0;
	}
	int ShadowRenderer::render()
	{
		glViewport(0, 0, renderContext->SHADOW_WIDTH, renderContext->SHADOW_HEIGHT);
		glEnable(GL_DEPTH_TEST);
		//ConfigureShaderAndMatrices();
		auto lightCount = renderContext->lightingContext->countLight;
		auto lightingdatas = renderContext->lightingContext->lights;
		for (int i = 0; i < lightCount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, renderContext->depthMapFBO[i]);
			glClear(GL_DEPTH_BUFFER_BIT);
			auto size = renderContext->renderQueue->get_size();
			while (size > 0)
			{
				auto lightData = lightingdatas[i];
				auto buffer = renderContext->renderQueue->get();
				//activate shader
				shadow_mapping_shader->use();

				glm::mat4 model = buffer->model;
				shadow_mapping_shader->setMat4("model", model);
				shadow_mapping_shader->setMat4("lightSpaceMatrix", lightData.lightSpaceMatrix);

				//optimation: combine all things into a vao then we just need to render once(because the shader is the same)
				glBindVertexArray(buffer->_VAO);
				glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				renderContext->renderQueue->pop();
				//push for next render
				renderContext->renderQueue->push(buffer);
				size--;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		return 0;
	}
	int ShadowRenderer::stop()
	{
		return 0;
	}
};