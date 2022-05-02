#include "ComponentsExporter.h"

namespace Engine {

	void ComponentsExporter::exportComp(sol::state& p_lua_state)
	{
		//todo 
		p_lua_state.new_usertype<entity>("entity",
			"x", &entity::changeX,
			"y", &entity::changeY,
			"z", &entity::changeZ
			);
	}

}