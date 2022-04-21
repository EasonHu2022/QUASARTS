#include "ScriptSystem.h"
#include "Scripts/ScriptsExporter.h"

#include <fstream>

namespace Engine {

	// singleton
	ScriptSystem* ScriptSystem::instance = nullptr;

	/// <summary>
	/// Create a singleton of the .
	/// </summary>
	/// <returns> The unique instance of the Scripts system. </returns>
	ScriptSystem* ScriptSystem::Instance()
	{
		if (nullptr == instance)
			instance = new ScriptSystem();
		return instance;
	}

	/// <summary>
	/// init
	/// </summary>
	void ScriptSystem::init()
	{
		createState();

		//add script component mask
		quasarts_component_mask temp;
		temp.mask = 0;
		temp.mask += (uint64_t)1 << COMPONENT_SCRIPT;
		add_component_mask(temp);
	}

	/// <summary>
	/// start
	/// </summary>
	/// <returns>res</returns>
	int ScriptSystem::start()
	{
		return 0;
	}

	/// <summary>
	/// update
	/// </summary>
	void ScriptSystem::update()
	{

		if (script_component->update_function)
		{
			(*(script_component->update_function))();
		}
	}

	/// <summary>
	/// stop
	/// </summary>
	/// <returns>res</returns>
	int ScriptSystem::stop()
	{
		return 0;
	}

	/// <summary>
	/// release
	/// </summary>
	void ScriptSystem::release()
	{
		if (script_component)
		{
			delete script_component;
			script_component = nullptr;
		}

	}

	void ScriptSystem::createState()
	{
		if (!lua_state)
		{
			lua_state = std::make_unique<sol::state>();
			//lua_state->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
			lua_state->open_libraries();  //get all libraries
			ScriptsExporter::exportScripts(*lua_state);
		}

		script_component = new ScriptComponent();
	}

	void ScriptSystem::destroyState()
	{
		if (lua_state)
		{
			lua_state.reset();
		}
	}

	void ScriptSystem::createScript(const std::string& file_name, const std::string& file_path)
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
			"end" << std::endl;
		ofs.close();

	}

	void ScriptSystem::loadScript(const std::string& path)
	{
		script_component->L = std::make_shared<sol::protected_function_result>(lua_state->script_file(path));
	}

	void ScriptSystem::loadScript(const std::string& path, ScriptComponent* component)
	{
		component->L = std::make_shared<sol::protected_function_result>(lua_state->script_file(path));
	}

	void ScriptSystem::reloadScript()
	{
		if (!script_path.empty())
		{
			loadScript(script_path);
		}
		else
		{
			loadScript(".\\Assets\\Scripts\\test.lua");
		}
	}

	void ScriptSystem::deleteScript()
	{
		if (!script_path.empty())
		{
			if (std::remove(script_path.c_str()) == 0)
			{
				QDEBUG("Deleted the file: {0}.lua", script_name);
			}
		}
		else
		{
			QWARN("No Script exists");
		}

	}
	void ScriptSystem::importUpdate()
	{
		if (!is_imported)
		{
			script_component->update_function = std::make_shared<sol::function>((*lua_state)["onUpdate"]);
			is_imported = true;
		}
	}
	std::string ScriptSystem::getScriptPath()
	{
		if (!script_path.empty())
		{
			return script_path;
		}
		QWARN("failed to get the script path");
		return "";
	}
	std::string ScriptSystem::getScriptName()
	{
		if (!script_name.empty())
		{
			return script_name;
		}
		QWARN("failed to get the script name");
		return "";
	}

}