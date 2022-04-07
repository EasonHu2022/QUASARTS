#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <string>

#include "Core/IManager.h"

//export functions to lua
#include "Logger/LogModule.h"
#include "Core/Input.h"
//export functions to lua


namespace Engine {
	class QS_API ScriptsSys : public IManager
	{
		// singleton
	private:
		static ScriptsSys* instance;
		ScriptsSys() {};
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
		void createScript(const std::string& file_name);		
		void loadScript(const std::string& path);
		void updateScript();
		void deleteScript();

		//export functions
		void exportLog();
		//export functions 

	public:
		//lua virtual machine
		sol::state lua{};
		std::string script_name = "";
		std::string root = "../Scripts/";
	};
}
