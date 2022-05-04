#pragma once
#include <sol/sol.hpp>


namespace Engine {

	class InputExporter
	{
	public:
		//export input related function
		static void exportInput(sol::state& p_lua_state);
	};
}





