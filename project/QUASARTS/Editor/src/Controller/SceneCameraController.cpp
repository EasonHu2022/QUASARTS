#include "SceneCameraController.h"

void SceneCameraController::on_update()
{
	auto camera = Engine::Application::Instance->scene->get_camera();
	if (camera != TOO_MANY_ENTITIES)
	{
		if (Engine::Input::mouseMoving)
		{
			//todo: if mousepos is in the scene window range
			//don't know the scene window range
			if (Engine::Input::get_mouse(1) /*&& Engine::Input::get_key(Q_KEY_LEFT_CONTROL)*/ )
			{
				auto transform = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(camera, COMPONENT_TRANSFORM);
				
				auto mouseMotion = Engine::Input::mouseMotion;
				auto x = transform->rotation.x;
				auto y = transform->rotation.y;
				x += MouseSensitivity * mouseMotion.y;
				y += MouseSensitivity * mouseMotion.x;
				//QDEBUG("rot camera");
				transform->rotation = { x,y,0.0f };
				Engine::ECSManager::Instance()->replace_component(camera, COMPONENT_TRANSFORM, *transform);
			}
		}
		
		
	}

}

void SceneCameraController::on_init()
{
}

void SceneCameraController::on_stop()
{
}
