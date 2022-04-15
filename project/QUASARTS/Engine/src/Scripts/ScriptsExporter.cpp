#include "ScriptsExporter.h"

//export 
#include "LogExporter.h"
#include "MathExporter.h"
#include "InputExporter.h"
#include "ComponentsExporter.h"
//export

namespace Engine {

	void ScriptsExporter::exportScripts(sol::state& p_lua_state)
	{
		sol::state& L = p_lua_state;
		LogExporter::exportLog(L);
		MathExporter::exportMath(L);
		InputExporter::exportInput(L);
		ComponentsExporter::exportComp(L);		
	}

}