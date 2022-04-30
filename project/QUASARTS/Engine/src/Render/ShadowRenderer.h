#pragma once
#include "glfw3.h"
#include "glad.h"
#include "Core/Core.h"
#include "Render/IRenderer.h"
#define MAX_LIGHTING_COUNT 5

namespace Engine
{     
	

	class QS_API ShadowRenderer : public IRenderer
	{
	private:
		static ShadowRenderer* instance;

	public:
		static ShadowRenderer* Instance();
		ShadowRenderer(RenderContext* renderContext);
		~ShadowRenderer();

	private:
		//shadow mapping vertex shader path
		const std::string vshPath = "F:\\WorkSpace\\QSEngine\\QUASARTS\\project\\QUASARTS\\Engine\\src\\Shader\\ShadowMapping.vsh";
		//shadow mapping fragment shader path
		const std::string fshPath = "F:\\WorkSpace\\QSEngine\\QUASARTS\\project\\QUASARTS\\Engine\\src\\Shader\\ShadowMapping.fsh";
		//shadow mapping shader
		Shader* shadow_mapping_shader = nullptr;
	public:
		/// <summary>
		/// init the memoryModule
		/// </summary>
		//int init(int width, int height, char* title);
		int init() override;

		/// <summary>
		/// update
		/// </summary>
		int render() override;

		/// <summary>
		/// stop the Memory module
		/// </summary>
		/// <returns></returns>
		int stop() override;

		/// <summary>
		/// release the memory module
		/// </summary>
		void release() override {};

	};

};

