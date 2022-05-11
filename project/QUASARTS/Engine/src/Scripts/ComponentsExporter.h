#pragma once
#include <sol/sol.hpp>
#include <iostream>
//component
#include "ECS/Component/TransformComponent.h"
#include "ECS/ECSManager.h"
//#include "glm/glm.hpp"

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

		static void updatePosition(unsigned int id, glm::vec3 dT)
		{
			Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
			trs->position = trs->position + dT;
		}

		static void updateRotation(unsigned int id, glm::vec3 dR)
		{
			Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
			trs->rotation = trs->rotation + dR;
		}

		static void updateScale(unsigned int id, glm::vec3 dS)
		{
			Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
			trs->scale = trs->scale + dS;
		}

	};


	


}