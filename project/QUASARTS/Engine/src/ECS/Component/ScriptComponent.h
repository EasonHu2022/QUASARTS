#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

#include <string>
#include <sol/sol.hpp>
#include <memory>

namespace Engine {

	struct QS_API ScriptComponent
	{

		std::shared_ptr<sol::protected_function_result> L;	// state handle
		std::shared_ptr<sol::function> update_function;		// control the onUpdate function in Lua side
		std::string script_name;							
		std::string script_path;																					 
	};

	// Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, ScriptComponent &script) {
		// Use the name and path, nothing else:
        inStream >> script.script_name >> script.script_path;

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const ScriptComponent &script) {
		// Use the name and path, nothing else:
        outStream << script.script_name << " " << script.script_path;
        
        return outStream;
    }
}