#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <cstdint>

#include "Core/IManager.h"
//#include "ECS/Component/ScriptComponent.h"



namespace Engine {
	class QS_API ScriptsSys : public IManager
	{
		// singleton
	private:
		static ScriptsSys* instance;
		ScriptsSys() : script_name(""), script_path(""), is_imported(false) {};
	public:
		static ScriptsSys* Instance();
		~ScriptsSys() {};

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
		void reloadScript();
		void deleteScript();
		void importUpdate();


	private:
		//lua virtual machine
		std::unique_ptr<sol::state> lua_state;
		sol::function lua_update;	//update function from lua side
		std::string script_name;
		std::string script_path;
		bool is_imported;			//check if the update function is imported to C++ side

		//std::unordered_map<std::string, ScriptComponent>;
	};
}