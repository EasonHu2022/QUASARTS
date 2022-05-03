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

		void createState();
		void destroyState();
		void createScript(const std::string& file_name, const std::string& file_path);
		void loadScript(const std::string& path);
		void loadScripts();
		void setScriptState(ScriptComponent* component);
		void reloadScript();
		void deleteScript();
		void deleteAllScripts();
		void refreshScript();
		void importFunc();
		void importFunc(ScriptComponent* component);
		void onUpdate();
		void onUpdate(ScriptComponent* component);
		std::string getScriptPath();
		std::string getScriptName();
		void setScriptPath(const std::string& path);
		void setComponentPath(ScriptComponent* component);
		void setScriptName(const std::string& name);
		void setComponentName(ScriptComponent* component);
		void addScriptComponent(ScriptComponent* component);

	private:
		std::unique_ptr<sol::state> lua_state; 		    //lua virtual machine
		std::string script_name;
		std::string script_path;
		//bool is_imported;							    //check if the update function is imported to C++ side
		std::vector<ScriptComponent*> script_components; //store all components
	};

}