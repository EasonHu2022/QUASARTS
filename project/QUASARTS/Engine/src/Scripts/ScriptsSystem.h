#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <string>
#include <memory>

#include "Core/IManager.h"

//export functions to lua
//#include "Logger/LogModule.h"
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

		void createContext();
		void destroyContext();
		void createScript(const std::string& file_name, const std::string& file_path);
		void loadScript(const std::string& path);
		void reloadScript();
		void deleteScript();

	private:
		//lua virtual machine
		std::unique_ptr<sol::state> lua_state;
		std::string script_name = "";
		std::string script_path = "";
	};
}
