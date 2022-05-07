#include "ParticleRenderer.h"
#include "ResourceManager/FileModule.h"

namespace Engine
{
	ParticleRenderer::ParticleRenderer(RenderContext* renderContext) : IRenderer(renderContext)
	{
		//get default engine assets path
		std::string path = FileModule::Instance()->get_internal_assets_path();

		vshPath = path + "Shader\\Particle.vsh";
		fshPath = path + "Shader\\Particle.fsh";

		 paths = std::vector<std::string>
		{
			path + "Texture\\Skybox\\right.jpg",
			path + "Texture\\Skybox\\left.jpg",
			path + "Texture\\Skybox\\top.jpg",
			path + "Texture\\Skybox\\bottom.jpg",
			path + "Texture\\Skybox\\front.jpg",
			path + "Texture\\Skybox\\back.jpg",
		};
	}
	ParticleRenderer::~ParticleRenderer()
	{
	}
	int ParticleRenderer::init()
	{
		glGenVertexArrays(1, &particleVAO);
		glGenBuffers(1, &particleVBO);
		glBindVertexArray(particleVAO);
		glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glBindVertexArray(particleVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		particleShader = new Shader(vshPath.data(), fshPath.data());
		
		return 0;
	}

	int ParticleRenderer::render()
	{
		return 0;
	}

	//set skybox renderer at the last renderer to render
	int ParticleRenderer::render(std::vector<Particle> particles)
	{
		if ( particleShader == NULL) return 0;
		glBindFramebuffer(GL_FRAMEBUFFER, renderContext->frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);

		//give a clear color of the window
		//glClearColor(0.45f, 0.55f, 0.6f, 1.0f);

		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		particleShader->use();
		auto view = glm::mat4(glm::mat3(renderContext->cameraContext->get_view_matrix())); // remove translation from the view matrix
		for (Particle particle : particles) {
			glDrawArrays(GL_TRIANGLES, 0, 8);
			//updateModelView(view, particle.getPosition(), particle.getRotation(), particle.getScale());
			particleShader->setMat4("view", view);
			printf("%d, ", particles.size());
		}
		particleShader->setMat4("projection", renderContext->cameraContext->get_projection_matrix());
		
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS); // set depth function back to default
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return 0;
	}

	int ParticleRenderer::stop()
	{
		return 0;
	}

	void ParticleRenderer::release()
	{
	}

	void ParticleRenderer::updateModelView(glm::mat4 view, glm::vec3 pos, float rot, float scal) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
		model[0][0] = view[0][0];
		model[0][1] = view[1][0];
		model[0][2] = view[2][0];
		model[1][0] = view[0][1];
		model[1][1] = view[1][1];
		model[1][2] = view[2][1];
		model[2][0] = view[0][2];
		model[2][1] = view[1][2];
		model[2][2] = view[2][2];
		model = glm::rotate(model, rot, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scal, scal, scal));
		particleShader->setMat4("view", view * model);
	}
};

