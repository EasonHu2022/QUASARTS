#include "ParticleRenderer.h"
#include "ResourceManager/FileModule.h"

namespace Engine
{
	ParticleRenderer::ParticleRenderer(RenderContext* renderContext) : IRenderer(renderContext)
	{
		//get default engine assets path
		std::string path = FileModule::Instance()->get_internal_assets_path();

		vshPath = path + "Shader/Particle.vsh";
		fshPath = path + "Shader/Particle.fsh";

		texPath = path + "Texture\\floor.jpg";

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), &particleVertices, GL_STATIC_DRAW);
		glBindVertexArray(particleVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glBindVertexArray(0);
		particleTexture = new Texture2D(texPath);
		particleShader = new Shader(vshPath.data(), fshPath.data());
		
		return 0;
	}

	int ParticleRenderer::render()
	{
		return 0;
	}

	//set skybox renderer at the last renderer to render
	int ParticleRenderer::render(std::map<std::string, std::pair<ParticleTexture, std::vector<Particle>>> emitters)
	{
		if ( particleShader == NULL) return 0;
		glBindFramebuffer(GL_FRAMEBUFFER, renderContext->frameBufferObject);
		glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);

		//give a clear color of the window
		//glClearColor(0.45f, 0.55f, 0.6f, 1.0f);

		glViewport(0, 0, renderContext->RT_WIDTH, renderContext->RT_HEIGHT);

		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		particleShader->use();
		auto view = glm::mat4(glm::mat3(renderContext->cameraContext->get_view_matrix())); // remove translation from the view matrix
		if (!emitters.empty()) {
			for (auto& [key, value] : emitters) {
				
				value.first.texture->use(GL_TEXTURE0);
				GLint iTextureUniform = glGetUniformLocation(particleShader->ID, "particleTexture");
				GLint texoffset1 = glGetUniformLocation(particleShader->ID, "texOffset1");
				GLint texoffset2 = glGetUniformLocation(particleShader->ID, "texOffset2");
				GLint texinfo = glGetUniformLocation(particleShader->ID, "texCoordInfo");
				glUniform1i(iTextureUniform, 0);

				for (Particle particle : value.second) {
					glBindVertexArray(particleVAO);
					updateModelView(view, particle.getPosition(), particle.getRotation(), particle.getScale());
					glUniform2f(texoffset1, particle.texOffset1.x, particle.texOffset1.y);
					glUniform2f(texoffset2, particle.texOffset2.x, particle.texOffset2.y);
					glUniform2f(texinfo, (float)particle.getRows(), particle.blend);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
					glBindVertexArray(0);
				}

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		particleShader->setMat4("projection", renderContext->cameraContext->get_projection_matrix());
		
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
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

