#pragma once
#include <sol/sol.hpp>


namespace Engine {

	class LogExporter
	{
	public:
		//export log function
		static void exportLog(sol::state& p_lua_state);
	};
}