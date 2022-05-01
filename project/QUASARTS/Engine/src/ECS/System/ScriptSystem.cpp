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
		script_components.clear();

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
		//call the imported function from lua side
		onUpdate();

		//get manager
		ECSManager* mgr = get_manager();

		//get entity mask
		quasarts_entity_ID_mask* ent = get_entity_ID_mask(0);

		ScriptComponent *script;
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (ent->mask[i] == 1)
			{
				script = mgr->get_component<ScriptComponent>(i, COMPONENT_SCRIPT);
				mgr->replace_component<ScriptComponent>(i, COMPONENT_SCRIPT, *script);
			}
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
			"-- thiz: current entity id\n"
			"function onUpdate(thiz)\n"
			"end" << std::endl;
		ofs.close();

	}

	void ScriptSystem::loadScript(const std::string& path)
	{
		//script_component->L = std::make_shared<sol::protected_function_result>(lua_state->script_file(path));
	}

	void ScriptSystem::loadScripts()
	{
		for (auto sc : script_components)
		{
			lua_state->script_file(sc->script_path);
		}
	}

	void ScriptSystem::setScriptState(ScriptComponent* component)
	{
		component->L = std::make_shared<sol::protected_function_result>(lua_state->script_file(component->script_path));
		importFunc(component);
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
	void ScriptSystem::deleteAllScripts()
	{
		for (auto& sc : script_components)
		{
			if (std::remove(sc->script_path.c_str()) == 0);
			QDEBUG("Deleted the file: {0}", sc->script_path);
		}
	}
	void ScriptSystem::refreshScript()
	{
		destroyState();
		createState();
	}
	void ScriptSystem::importFunc()
	{
		//if (!is_imported)
		//{
			//script_component->update_function = std::make_shared<sol::function>((*lua_state)["onUpdate"]);
			//is_imported = true;
		//}		
	}

	void ScriptSystem::importFunc(ScriptComponent* component)
	{
		//if (!is_imported)
		//{
			component->update_function = std::make_shared<sol::function>((*lua_state)["onUpdate"]);
			//is_imported = true;
		//}
	}

	void ScriptSystem::onUpdate()
	{
		//if (script_component->update_function)
		//{
		//	(*(script_component->update_function))();
		//}

		//if (Engine::ECSManager::Instance()->get_current_entity() == 1)
		//{
		//	Engine::ScriptComponent* sc = Engine::ECSManager::Instance()->get_component<Engine::ScriptComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_SCRIPT);
		//	if (sc->update_function)
		//	{
		//		(*(sc->update_function))();
		//	}
		//}

		//loop all
		for (auto sc : script_components)
		{
			if (sc->update_function)
			{
				(*(sc->update_function))(sc->entity_id);
			}
		}


	}

	void ScriptSystem::onUpdate(ScriptComponent* component)
	{
		if (component->update_function)
		{
			(*(component->update_function))();
		}
	}

	std::string ScriptSystem::getScriptPath()
	{
		if (!script_path.empty())
		{
			return script_path;
		}
		//temp
		else {

			return ".\\Assets\\Scripts\\test.lua";
		}
		//QWARN("failed to get the script path");
	}
	std::string ScriptSystem::getScriptName()
	{
		if (!script_name.empty())
		{
			return script_name;
		}
		//temp
		else {

			return "test";
		}
		//QWARN("failed to get the script name");
	}

	void ScriptSystem::setScriptPath(const std::string& path)
	{
		script_path = path;
	}

	void ScriptSystem::setComponentPath(ScriptComponent* component)
	{
		//component->script_path = this->script_path;
		component->script_path = ".//Assets//Scripts//test.lua";
	}

	void ScriptSystem::setScriptName(const std::string& name)
	{
		script_name = name;
	}

	void ScriptSystem::setComponentName(ScriptComponent* component)
	{
		component->script_name = this->script_name;
	}

	void ScriptSystem::addScriptComponent(ScriptComponent* component)
	{
		script_components.push_back(component);
	}

}