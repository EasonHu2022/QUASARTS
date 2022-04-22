#pragma once
#include "ECS/System/System.h"
#include "Render/RenderQueue.h"
#include "Core/Core.h"
#include "Render/UniformBufferObject.h"
namespace Engine
{
	class QS_API RenderSystem : public System
	{
	public:
		RenderSystem() ;
		~RenderSystem() {};
	
	private:
		UniformBufferObject* matricesBuffer;
		UniformBufferObject* lightBuffer;

	public:
		void init() override;
		
		int start() override { return 0; };

		int stop() override { return 0; };

		void release() override { };

		void update() override;
		//update camera
		void update_camera();

		//update light
		void update_light();

	};

};