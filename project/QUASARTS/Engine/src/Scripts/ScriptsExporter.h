#pragma once
#include <sol/sol.hpp>

namespace Engine {

	class ScriptsExporter
	{
	public:
		//export all functions to lua environment
		static void exportScripts(sol::state& p_lua_state);
	};
}