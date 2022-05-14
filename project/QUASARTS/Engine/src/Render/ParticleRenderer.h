#pragma once
#include "Core/Core.h"
#include "IRenderer.h"
#include <vector>
#include "Shader.h"
#include "Particle.h"
namespace Engine
{

	class QS_API ParticleRenderer : public IRenderer
	{
    public:
        ParticleRenderer(RenderContext* renderContext);
        ~ParticleRenderer();
    public:
		int init() override;

        int render() override;

		int render(std::map<Texture2D*, std::vector<Particle>>);

		int stop() override;

		void release() override;

        void updateModelView(glm::mat4, glm::vec3, float, float);

    private:
        std::string vshPath;
        std::string fshPath;
        unsigned int particleVAO, particleVBO;
        float particleVertices[8] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f};
        Texture2D* particleTexture;
        Shader* particleShader = NULL;
        std::string texPath;
	};
};
