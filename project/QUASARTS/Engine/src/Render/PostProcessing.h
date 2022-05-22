#pragma once
#include "Core/Core.h"
#include "IRenderer.h"
#include <vector>
#include "Shader.h"
#include "CubeMap.h"
namespace Engine
{

	class QS_API PostProcessing : public IRenderer
	{
    public:
        PostProcessing(RenderContext* renderContext);
        ~PostProcessing();
    public:
		int init() override;

		int render() override;
        

		int stop() override;

		void release() override;

        //option
        int gaussianBlur();
    private:
        std::string vshPath;
        std::string fshPath;
        std::string vGaussianPath;
        std::string fGaussianPath;
        bool horizontal = true, first_iteration = true;
        unsigned int planeVAO, planeVBO;
        float plane[24] = {
           -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

         -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
        };
        Shader* postProcessingShader = NULL;
        Shader* GaussianBlur = NULL;

	};
};
