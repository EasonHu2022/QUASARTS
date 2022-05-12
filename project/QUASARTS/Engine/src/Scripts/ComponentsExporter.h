#pragma once
#include <sol/sol.hpp>
#include <iostream>
#include <string>
//component
#include "ECS/ECSManager.h"
#include "glm/glm.hpp"

namespace Engine {

	class ComponentsExporter
	{
	public:  
		//export components..   transform, camera ...
		//todo
		static void exportComp(sol::state& p_lua_state);
	};


	class entity {

	public:

		//transform
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


		//health
		static void setMaxHealth(unsigned int id, float maxH)
		{
			Engine::HealthComponent* health = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(id, COMPONENT_HEALTH);
			health->max_health = maxH;
		}


		static void setCurrentHealth(unsigned int id, float curr_health)
		{
			Engine::HealthComponent* health = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(id, COMPONENT_HEALTH);
			health->current_health = curr_health;
		}

		static float getCurrentHealth(unsigned int id)
		{
			Engine::HealthComponent* health = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(id, COMPONENT_HEALTH);
			return health->current_health;
		}


		//weapon 
		static void setDamage(unsigned int id, float damage)
		{
			Engine::WeaponComponent* weapon = Engine::ECSManager::Instance()->get_component<Engine::WeaponComponent>(id, COMPONENT_WEAPON);
			weapon->base_damage = damage;
		}

		static void setRange(unsigned int id, float range)
		{
			Engine::WeaponComponent* weapon = Engine::ECSManager::Instance()->get_component<Engine::WeaponComponent>(id, COMPONENT_WEAPON);
			weapon->attack_range = range;
		}

		static void setSpeed(unsigned int id, int speed)
		{
			Engine::WeaponComponent* weapon = Engine::ECSManager::Instance()->get_component<Engine::WeaponComponent>(id, COMPONENT_WEAPON);
			weapon->attack_speed = speed;
		}

	};

	static unsigned int createEntity(const std::string& entity_name)
	{
		unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
		Engine::ECSManager::Instance()->set_entityName(entityID, entity_name);
		
		return entityID;
	}

	static void addComponent(unsigned int entity_id, const std::string& component_type)
	{

		if (component_type == "Health")
		{
			Engine::ECSManager::Instance()->create_component<Engine::HealthComponent>(entity_id, COMPONENT_HEALTH);
		}

	}
	


}