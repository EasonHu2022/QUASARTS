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
		virtual void init() override;
		
		virtual void update() override;
		//update camera
		void update_camera();

		//update light
		void update_light();

	};

};