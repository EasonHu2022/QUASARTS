#pragma once
#include "ECS/System/System.h"
#include "Render/RenderQueue.h"
#include "Core/Core.h"

namespace Engine
{
	class QS_API RenderSystem : public System
	{
	public:
		RenderSystem() ;
		~RenderSystem() {};
	
	

	public:
		void init() override;
		
		int start() override { return 0; };

		int stop() override { return 0; };

		void release() override { };

		void update() override;
		//update camera
		void update_projection();

		//update light
		void update_light();

	};

};