#include "ComponentsExporter.h"

namespace Engine {

	void ComponentsExporter::exportComp(sol::state& p_lua_state)
	{
		//todo 
		p_lua_state.new_usertype<entity>("entity",

			//transform attributes
			"updatePosition", &entity::updatePosition,
			"updateRotation", &entity::updateRotation,
			"updateScale", &entity::updateScale,

			//health 
			"setMaxHealth", &entity::setMaxHealth,
			"setCurrentHealth", &entity::setCurrentHealth,
			"getCurrentHealth", &entity::getCurrentHealth,

			//weapon
			"setDamage", &entity::setDamage,
			"setRange", &entity::setRange,
			"setSpeed", &entity::setSpeed
			);

		p_lua_state.set_function("createEntity", &createEntity);
		p_lua_state.set_function("addComponent", &addComponent);

	}

}