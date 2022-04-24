#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <cstdint>

#include "System.h"
#include "Core/IManager.h"
#include "ECS/Component/ScriptComponent.h"
#include "QuasartsEngine.h"

namespace Engine {
	class QS_API ScriptSystem : public System
	{
		// singleton
	private:
		static ScriptSystem* instance;
		ScriptSystem() : script_name(""), script_path(""), is_imported(false), script_component(nullptr) {};
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
		void loadScript(const std::string& path, ScriptComponent* component);
		void reloadScript();
		void deleteScript();
		void importFunc();
		void onUpdate();
		void importFunc(ScriptComponent* component);
		std::string getScriptPath();
		std::string getScriptName();

	private:
		std::unique_ptr<sol::state> lua_state; 		//lua virtual machine
		std::string script_name;
		std::string script_path;
		bool is_imported;							//check if the update function is imported to C++ side
		ScriptComponent* script_component;

		//std::unordered_map<std::string, ScriptComponent>;
	};

}