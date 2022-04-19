#include "ScriptsSystem.h"
#include "ScriptsExporter.h"
#include "Logger/LogModule.h"

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
		createState();
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

		if (lua_update)
		{
		lua_update();
		}
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
	
	void ScriptsSys::createState()
	{
		if (!lua_state)
		{
			lua_state = std::make_unique<sol::state>();
			lua_state->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
			ScriptsExporter::exportScripts(*lua_state);
		}
	}

	void ScriptsSys::destroyState()
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
		script_path = file_path + sub_path + file_name + ".lua";
		std::ofstream ofs;
		ofs.open(script_path, std::ios::out);
		if (ofs)
		{
			QDEBUG("created and added the script: {0}.lua , the path is {1}", script_name, script_path);
		}
		ofs << "--Update the script here\n"
			   "function onUpdate()\n" 
			   "end"<< std::endl;
		ofs.close();
		
	}

	void ScriptsSys::loadScript(const std::string& path)
	{
		//auto ret = lua_state->script_file(path);
		lua_state->script_file(path);
	}
	
	void ScriptsSys::reloadScript()
	{		
		loadScript(script_path);
	}


	void ScriptsSys::deleteScript()
	{
		std::string file_path = script_path;

		if (std::remove(file_path.c_str()) == 0)
		{
			QDEBUG("Deleted the file: {0}.lua", script_name);
		}
	}
	void ScriptsSys::importUpdate()
	{
		if (!is_imported)
		{
			lua_update = (*lua_state)["onUpdate"];
			is_imported = true;
		}
	}
	std::string ScriptsSys::getScriptPath()
	{
		if (!script_path.empty())
		{
			return script_path;
		}
		QWARN("failed to get the script path");
		return "";
	}
	std::string ScriptsSys::getScriptName()
	{
		if (!script_name.empty())
		{
			return script_name;
		}
		QWARN("failed to get the script name");
		return "";
	}
}

//sol::lib::base,
//sol::lib::package,
//sol::lib::string,
//sol::lib::math,
//sol::lib::table