#include "ScriptSystem.h"
#include "Scripts/ScriptsExporter.h"

#include <fstream>
#include <sys/stat.h>

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
		//call the imported function from lua side
		onUpdate();
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

	//open lua state
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

	//destroy lua state for reload
	void ScriptSystem::destroyState()
	{
		lua_state.reset();
	}

	//create a default lua file
	void ScriptSystem::createScript(const std::string& file_name, const std::string& file_path)
	{
		script_name = file_name;
		script_path = file_path + "/" +file_name + ".lua";
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

	//load all entities' script components
	void ScriptSystem::loadScripts()
	{
		auto script_components = getExistingComponents();

		if (script_components.empty())
		{
			QWARN("No script components here, please check it.");
		}
		else
		{
			for (auto sc : script_components)
			{
				if(isScriptExists(sc->script_path))
				{
					lua_state->script_file(sc->script_path);
					registerFunction(sc);
				}
				else
				{
					QWARN("failed to run the script, check if the script exists");
				}
			}
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
		auto script_components = getExistingComponents();
		for (auto& sc : script_components)
		{
			if (std::remove(sc->script_path.c_str()) == 0);
			QDEBUG("Deleted the file: {0}", sc->script_path);
		}
	}

	//"stop" the script context
	void ScriptSystem::refreshScript()
	{
		unregisterAllFunction();
		destroyState();
		createState();
	}

	//register current entity's script function from lua side
	void ScriptSystem::registerFunction(ScriptComponent* component)
	{
		//if (!is_imported)
		//{
		component->update_function = std::make_shared<sol::function>((*lua_state)["onUpdate"]);
		//is_imported = true;
	//}
	}

	//register all entities function
	void ScriptSystem::registerAllFunction()
	{
		auto script_components = getExistingComponents();
		for (auto& sc : script_components)
		{
			sc->update_function = std::make_shared<sol::function>((*lua_state)["onUpdate"]);
		}
	}

	//unregister 
	void ScriptSystem::unregisterFunction(ScriptComponent* component)
	{
		component->update_function.reset();
	}

	//unregister 
	void ScriptSystem::unregisterAllFunction()
	{
		auto script_components = getExistingComponents();

		for (auto sc : script_components)
		{
			sc->update_function.reset();
		}
	}

	//run the functions from scripts
	void ScriptSystem::onUpdate()
	{	
		auto script_components = getExistingComponents();

		//loop all
		for (auto sc : script_components)
		{
			if (sc->update_function)
			{
				(*(sc->update_function))(sc->entity_id, TimeModule::Instance()->get_frame_delta_time().sec());
			}
			//
		}
	}

	void ScriptSystem::onUpdate(ScriptComponent* component)
	{
		if (component->update_function)
		{
			(*(component->update_function))(component->entity_id, TimeModule::Instance()->get_frame_delta_time().sec());
		}
	}

	//initialize script component -- set path and entity id
	void ScriptSystem::initComponent(ScriptComponent* component, const std::string& comp_path, unsigned int id)
	{
			component->entity_id = id;
			component->script_path = comp_path;
	}

	//check if the lua file exists
	bool ScriptSystem::isScriptExists(std::string path)
	{
		struct stat buffer;
		return(stat(path.c_str(), &buffer) == 0);
	}

	//get all script components from existing entities
	std::vector<ScriptComponent*> ScriptSystem::getExistingComponents()
	{

		//store current entities which have script component
		std::vector<ScriptComponent*> current;

		//get manager
		ECSManager* mgr = get_manager();

		//get entity mask
		quasarts_entity_ID_mask* ent = get_entity_ID_mask(0);


		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			ScriptComponent* script;
			if (ent->mask[i] == 1)
			{
				script = mgr->get_component<ScriptComponent>(i, COMPONENT_SCRIPT);
				current.push_back(script);
			}
		}
		return current;
	}


	std::string ScriptSystem::getScriptPath()
	{
		if (!script_path.empty())
		{
			return script_path;
		}
		else 
		{

			QWARN("failed to get the script path");
		}
	}
	std::string ScriptSystem::getScriptName()
	{
		if (!script_name.empty())
		{
			return script_name;
		}
		else
		{

			QWARN("failed to get the script name");
		}
	}


	void ScriptSystem::setScriptPath(const std::string& path)
	{
		script_path = path;
	}

	void ScriptSystem::setComponentPath(ScriptComponent* component)
	{
		if (!script_path.empty())
		{
			component->script_path = script_path;
		}
	}

	void ScriptSystem::setScriptName(const std::string& name)
	{
		script_name = name;
	}
}