#include "ScriptsSystem.h"
#include "Logger/LogModule.h"

#include "Utilities/FileParser.h"

namespace Engine {

	// singleton
	ScriptsSys* ScriptsSys::instance = nullptr;
	
	/// <summary>
	/// Create a singleton of the .
	/// </summary>
	/// <returns> The unique instance of the Scripts system. </returns>
	ScriptsSys* ScriptsSys::Instance()
	{
		if (nullptr == instance)
			instance = new ScriptsSys();
		return instance;
	}
		
	/// <summary>
	/// init
	/// </summary>
	void ScriptsSys::init()
	{
		lua.open_libraries(sol::lib::base);
		exportLogSys();
	}

	/// <summary>
	/// start
	/// </summary>
	/// <returns>res</returns>
	int ScriptsSys::start()
	{
		//get file name test 
		std::string file_name = getFileName("./test.lua");
		QDEBUG("Test : Get file name func : {0}", file_name);

		//scripts test
		QDEBUG("Scripts system test :  engine hardcode");
 		loadScripts("test.lua");
		return 0;
	}

	/// <summary>
	/// update
	/// </summary>
	void ScriptsSys::update()
	{

	}

	/// <summary>
	/// stop
	/// </summary>
	/// <returns>res</returns>
	int ScriptsSys::stop()
	{
		return 0;
	}

	/// <summary>
	/// release
	/// </summary>
	void ScriptsSys::release()
	{

	}
	
	void ScriptsSys::loadScripts(const std::string& path)
	{
		lua.script_file(path);
	}
	
	void ScriptsSys::exportFunction(const std::string& lua_func_name, void(*func)(const std::string))
	{
		lua.set_function(lua_func_name, &(*func));
	}
	
	void ScriptsSys::exportFunction(const std::string& lua_func_name, int(*func)())
	{
		lua.set_function(lua_func_name, &(*func));
	}

	void ScriptsSys::exportLogSys()
	{
		//for engine 
		lua.set_function("Qlog", std::function <void(const std::string&)>([](const std::string& str) {
			QDEBUG(str);
			}));

		lua.set_function("Qtrace", std::function <void(const std::string&)>([](const std::string& str) {
			QTRACE(str);
			}));
		lua.set_function("Qerror", std::function <void(const std::string&)>([](const std::string& str) {
			QERROR(str);
			}));

		//for game     
		lua.set_function("Glog", std::function <void(const std::string&)>([](const std::string& str) {
			DEBUG(str);
			}));
		lua.set_function("Gtrace", std::function <void(const std::string&)>([](const std::string& str) {
			TRACE(str);
			}));
		lua.set_function("Gerror", std::function <void(const std::string&)>([](const std::string& str) {
			ERROR(str);
			}));

	}
}



//ScriptsSys::ScriptsSys()
//{
//	lua.open_libraries(sol::lib::base,
//		sol::lib::package,
//		sol::lib::string,
//		sol::lib::math,
//		sol::lib::table
//	);
//
//}