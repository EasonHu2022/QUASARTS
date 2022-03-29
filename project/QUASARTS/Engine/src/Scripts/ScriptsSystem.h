#pragma once
#include <lua.hpp>
#include <sol/sol.hpp>
#include <iostream>

#include "Core/IManager.h"
#include "Logger/LogModule.h"

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
		void loadScripts(const std::string& path);

		//todo  template..  if we need     or we could have the export.cpp
		void exportFunction(const std::string& lua_func_name, void(*func)(const std::string));

		void exportFunction(const std::string& lua_func_name, int(*func)());
		//todo ... more 


	//public status for now 
	public:
		//load the lua virtual machine, which is wrapped by sol2. 
		sol::state lua{};
	};
}
