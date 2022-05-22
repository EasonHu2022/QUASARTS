#include "PostProcessing.h"
#include "ResourceManager/FileModule.h"

namespace Engine
{
	PostProcessing::PostProcessing(RenderContext* renderContext) : IRenderer(renderContext)
	{
		//get default engine assets path
		std::string path = FileModule::Instance()->get_internal_assets_path();

		vshPath = path + "Shader/postProcessing.vsh";
		fshPath = path + "Shader/postProcessing.fsh";
		vGaussianPath = path + "Shader/GaussianBlur.vsh";
		fGaussianPath = path + "Shader/GaussianBlur.fsh";
	}
	PostProcessing::~PostProcessing()
	{
	}
	int PostProcessing::init()
	{
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(plane), &plane, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		
		postProcessingShader = new Shader(vshPath.data(), fshPath.data());
		GaussianBlur = new Shader(vGaussianPath.data(), fGaussianPath.data());;
		return 0;
	}

	//set skybox renderer at the last renderer to render
	int PostProcessing::render()
	{
		if (postProcessingShader == NULL) return 0;
		glBindFramebuffer(GL_FRAMEBUFFER, renderContext->finalBufferObject);
		glDepthMask(GL_FALSE);
		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

	
		postProcessingShader->use();
		
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderContext->colorTexture);
		GLint iTextureUniform = glGetUniformLocation(postProcessingShader->ID, "postTexture");
		glUniform1i(iTextureUniform, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderContext->screenTexture[!horizontal]);
		postProcessingShader->setFloat("exposure", 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS); // set depth function back to default
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return 0;
	}

	int PostProcessing::stop()
	{
		return 0;
	}

	void PostProcessing::release()
	{
	}
	int PostProcessing::gaussianBlur()
	{
		first_iteration = true;
		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);
		unsigned int amount = 10;
		GaussianBlur->use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, renderContext->postBufferObject[horizontal]);
			GaussianBlur->setInt("horizontal", horizontal);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? renderContext->emissionTeture : renderContext->screenTexture[!horizontal]);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return 0;
	}
};

