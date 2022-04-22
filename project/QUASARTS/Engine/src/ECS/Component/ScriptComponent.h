#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

#include <string>
#include <sol/sol.hpp>
#include <memory>

namespace Engine {

	struct ScriptComponent
	{

		std::shared_ptr<sol::protected_function_result> L;	// state handle
		std::shared_ptr<sol::function> update_function;		// control the onUpdate function in Lua side
		std::string script_name;							// not use for now
		std::string script_path;							// not use for now
														 
	};
}