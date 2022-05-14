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


	//Util

	//Camera
	static unsigned int getCamera();
	static void camera_zoom_in(unsigned int id);
	static void camera_zoom_out(unsigned int id);

	//transform
	static void updatePosition(unsigned int id, glm::vec3 dT);
	static void updateRotation(unsigned int id, glm::vec3 dR);
	static void updateScale(unsigned int id, glm::vec3 dS);
	static glm::vec3 getPosition(unsigned int id);

	//health
	static void setMaxHealth(unsigned int id, float maxH);
	static void setCurrentHealth(unsigned int id, float curr_health);
	static float getCurrentHealth(unsigned int id);

	//weapon 
	static void setDamage(unsigned int id, float damage);
	static void setRange(unsigned int id, float range);
	static void setSpeed(unsigned int id, int speed);

	//create new entity and add components
	static unsigned int createEntity(const std::string& entity_name);
	static void addComponent(unsigned int entity_id, const std::string& component_type);


}