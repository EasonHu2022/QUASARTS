#pragma once
#include "Core/Core.h"
#include "IRenderer.h"
#define MAX_LIGHTING_COUNT 5

namespace Engine
{     




	class QS_API MeshRenderer : public IRenderer
	{
	private:


	public:

		MeshRenderer(RenderContext *renderContext);
		~MeshRenderer();


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
		void release() override  {};

	};

};

