#pragma once
#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Engine {

	class MathExporter
	{
	public:
		//export glm function
		static void exportMath(sol::state& p_lua_state);
	};
}