#include "SkyboxRenderer.h"
#include "ResourceManager/FileModule.h"

namespace Engine
{
	SkyBoxRenderer::SkyBoxRenderer(RenderContext* renderContext) : IRenderer(renderContext)
	{
		//get default engine assets path
		std::string path = FileModule::Instance()->get_internal_assets_path();

		vshPath = path + "Shader/SkyBox.vsh";
		fshPath = path + "Shader/SkyBox.fsh";

		 paths = std::vector<std::string>
		{
			path + "Texture/Skybox/right.jpg",
			path + "Texture/Skybox/left.jpg",
			path + "Texture/Skybox/top.jpg",
			path + "Texture/Skybox/bottom.jpg",
			path + "Texture/Skybox/front.jpg",
			path + "Texture/Skybox/back.jpg",
		};
	}
	SkyBoxRenderer::~SkyBoxRenderer()
	{
	}
	int SkyBoxRenderer::init() 
	{
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		cubeMapTexture = new CubeMap(paths);
		skyBoxShader = new Shader(vshPath.data(), fshPath.data());
		
		return 0;
	}

	//set skybox renderer at the last renderer to render
	int SkyBoxRenderer::render()
	{
		if (cubeMapTexture == NULL || skyBoxShader == NULL) return 0;
		glBindFramebuffer(GL_FRAMEBUFFER, renderContext->frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);

		//give a clear color of the window
		//glClearColor(0.45f, 0.55f, 0.6f, 1.0f);

		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyBoxShader->use();
		auto view = glm::mat4(glm::mat3(renderContext->cameraContext->get_view_matrix())); // remove translation from the view matrix
		skyBoxShader->setMat4("view", view);
		skyBoxShader->setMat4("projection", renderContext->cameraContext->get_projection_matrix());
		// skybox cube
		glBindVertexArray(skyboxVAO);
		cubeMapTexture->use(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS); // set depth function back to default
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return 0;
	}

	int SkyBoxRenderer::stop()
	{
		return 0;
	}

	void SkyBoxRenderer::release()
	{
	}
};

