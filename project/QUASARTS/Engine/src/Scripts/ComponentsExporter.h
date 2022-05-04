#pragma once
#include <sol/sol.hpp>
#include <iostream>
//component
#include "ECS/Component/TransformComponent.h"
#include "ECS/ECSManager.h"

namespace Engine {

	class ComponentsExporter
	{
	public:  
		//export components..   transform, camera ...
		//todo
		static void exportComp(sol::state& p_lua_state);
	};

	//transform
	//test func
	class entity {

	public:
		static void changeX(unsigned int id, float step)
		{
			//cannot work
			Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
			trs->position.x += step;
		}

		static void changeY(unsigned int id, float step)
		{
			Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
			trs->position.y += step;
		}

		static void changeZ(unsigned int id, float step)
		{
			Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
			trs->position.z += step;
		}
	};


	


}