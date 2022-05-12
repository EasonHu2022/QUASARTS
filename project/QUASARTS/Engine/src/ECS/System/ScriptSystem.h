#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <cstdint>
#include <vector>


#include "System.h"
#include "ECS/Component/ScriptComponent.h"
#include "ECS/ECSManager.h"
#include "QuasartsEngine.h"

namespace Engine {
	class QS_API ScriptSystem : public System
	{
		// singleton
	private:
		static ScriptSystem* instance;
		ScriptSystem() : script_name(""), script_path("") {};
	public:
		static ScriptSystem* Instance();
		~ScriptSystem() {};

	public:
		void init();
		int start();
		void update();
		int stop();
		void release();

		// Usage functions //
	public:

		//open lua state
		void createState();

		//destroy lua state for reload
		void destroyState();

		//create a default lua file
		void createScript(const std::string& file_name, const std::string& file_path);

		//load all entities' script components
		void loadScripts();

		//--------delete lua file 
		void deleteScript();
		void deleteAllScripts();
		//-------------------------

		//"stop" the script context
		void refreshScript();

		//register current entity's script function from lua side
		void registerFunction(ScriptComponent* component);
		//register all entities function
		void registerAllFunction();

		//unregister 
		void unregisterFunction(ScriptComponent* component);
		void unregisterAllFunction();

		//run the onUpdate function from scripts
		void onUpdate();
		void onUpdate(ScriptComponent* component);

		//run the onInit function from scripts
		void onInit();
		void onInit(ScriptComponent* component);

		//initialize script component -- set path and entity id
		void initComponent(ScriptComponent* component, const std::string& comp_path, unsigned int id);

		//check if the lua file exists
		bool isScriptExists(std::string path);

		//get all script components from existing entities
		std::vector<ScriptComponent*> getExistingComponents();

		//
		std::string getScriptPath();
		std::string getScriptName();
		void setScriptPath(const std::string& path);
		void setComponentPath(ScriptComponent* component);
		void setScriptName(const std::string& name);

	private:
		std::unique_ptr<sol::state> lua_state; 		    //lua virtual machine
		std::string script_name;
		std::string script_path;
	};

}