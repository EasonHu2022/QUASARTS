#pragma once
#include "glfw3.h"
#include "glad.h"
#include "Core/Core.h"
#include "RenderContext.h"
#define MAX_LIGHTING_COUNT 5

namespace Engine
{     


	class QS_API IRenderer
	{
	protected:
		RenderContext* renderContext;
	public:
		IRenderer(RenderContext* _renderContext) :renderContext(_renderContext) {}
		~IRenderer() {};

		virtual int init() = 0;

		
		virtual int render() = 0;

	
		virtual int stop() = 0;

	
		virtual void release() = 0;
	};

};

