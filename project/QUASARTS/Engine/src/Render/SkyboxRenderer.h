#pragma once
#include "Core/Core.h"
#include "IRenderer.h"
#include <vector>
#include "Shader.h"
#include "CubeMap.h"
namespace Engine
{

	class QS_API SkyBoxRenderer : public IRenderer
	{
    public:
        SkyBoxRenderer(RenderContext* renderContext);
        ~SkyBoxRenderer();
    public:
		int init() override;

		int render() override;

		int stop() override;

		void release() override;

    private:
        //std::string vshPath = "F:\\WorkSpace\\QSEngine\\QUASARTS\\project\\QUASARTS\\Assets\\Shader\\SkyBox.vsh";
        //std::string fshPath = "F:\\WorkSpace\\QSEngine\\QUASARTS\\project\\QUASARTS\\Assets\\Shader\\SkyBox.fsh";
        std::string vshPath = "..\\Assets\\Shader\\SkyBox.vsh";
        std::string fshPath = "..\\Assets\\Shader\\SkyBox.fsh";
        unsigned int skyboxVAO, skyboxVBO;
        float skyboxVertices[108] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        Shader* skyBoxShader = NULL;
        CubeMap* cubeMapTexture = NULL;
        std::vector<std::string> paths
        {
            "..\\Assets\\Texture\\right.jpg",
            "..\\Assets\\Texture\\left.jpg",
            "..\\Assets\\Texture\\top.jpg",
            "..\\Assets\\Texture\\bottom.jpg",
            "..\\Assets\\Texture\\front.jpg",
            "..\\Assets\\Texture\\back.jpg",
        };
	};
};
