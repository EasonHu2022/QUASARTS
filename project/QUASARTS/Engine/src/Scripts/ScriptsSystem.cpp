#include "ScriptsSystem.h"
#include "ScriptsExporter.h"
#include "Utilities/FileParser.h"

#include <fstream>

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
		createContext();
	}

	/// <summary>
	/// start
	/// </summary>
	/// <returns>res</returns>
	int ScriptsSys::start()
	{
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
	
	void ScriptsSys::createContext()
	{
		if (!lua_state)
		{
			lua_state = std::make_unique<sol::state>();
			lua_state->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
			ScriptsExporter::exportScripts(*lua_state);
		}
	}

	void ScriptsSys::destroyContext()
	{
		if (lua_state)
		{
			lua_state.reset();
		}
	}

	void ScriptsSys::createScript(const std::string& file_name, const std::string& file_path)
	{	
		script_name = file_name;
		std::string sub_path = "\\Assets\\Scripts\\";
		script_path = file_path + sub_path + file_name;
		std::ofstream ofs;
		ofs.open(script_path + ".lua", std::ios::out);
		if (ofs)
		{
			QDEBUG("created and added the script: {0}.lua , the path is {1}", script_name, script_path);
		}
		ofs << "--Scripts: we now can use the log function" << std::endl;
		ofs << "--Hint: Qlog(), Qtrace(), Qerror()...etc" << std::endl;
		ofs.close();
		
	}

	void ScriptsSys::loadScript(const std::string& path)
	{
		lua_state->script_file(path);
		
	}
	
	void ScriptsSys::reloadScript()
	{
		loadScript(script_path + ".lua");
	}

	void ScriptsSys::deleteScript()
	{
		std::string file_path = script_path + ".lua";

		if (std::remove(file_path.c_str()) == 0)
		{
			QDEBUG("Deleted the file: {0}.lua", script_name);
		}

	}
}

//sol::lib::base,
//sol::lib::package,
//sol::lib::string,
//sol::lib::math,
//sol::lib::table