//#include "ScriptsSystem.h"
//#include "ScriptsExporter.h"
//#include "Logger/LogModule.h"
//
//#include <fstream>
//
//namespace Engine {
//
//	// singleton
//	ScriptsSys* ScriptsSys::instance = nullptr;
//	
//	/// <summary>
//	/// Create a singleton of the .
//	/// </summary>
//	/// <returns> The unique instance of the Scripts system. </returns>
//	ScriptsSys* ScriptsSys::Instance()
//	{
//		if (nullptr == instance)
//			instance = new ScriptsSys();
//		return instance;
//	}
//		
//	/// <summary>
//	/// init
//	/// </summary>
//	void ScriptsSys::init()
//	{
//		createState();
//	}
//
//	/// <summary>
//	/// start
//	/// </summary>
//	/// <returns>res</returns>
//	int ScriptsSys::start()
//	{
//		return 0;
//	}
//
//	/// <summary>
//	/// update
//	/// </summary>
//	void ScriptsSys::update()
//	{
//		
//		if (script_component->update_function)
//		{
//			script_component->update_function();
//		}
//	}
//
//	/// <summary>
//	/// stop
//	/// </summary>
//	/// <returns>res</returns>
//	int ScriptsSys::stop()
//	{
//		return 0;
//	}
//
//	/// <summary>
//	/// release
//	/// </summary>
//	void ScriptsSys::release()
//	{
//		if (script_component)
//		{
//			delete script_component;
//			script_component = nullptr;
//		}
//
//	}
//	
//	void ScriptsSys::createState()
//	{
//		if (!lua_state)
//		{
//			lua_state = std::make_unique<sol::state>();
//			//lua_state->open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
//			lua_state->open_libraries();  //get all libraries
//			ScriptsExporter::exportScripts(*lua_state);
//		}
//
//		script_component = new ScriptComponent();
//	}
//
//	void ScriptsSys::destroyState()
//	{
//		if (lua_state)
//		{
//			lua_state.reset();
//		}
//	}
//
//	void ScriptsSys::createScript(const std::string& file_name, const std::string& file_path)
//	{	
//		script_name = file_name;
//		std::string sub_path = "\\Assets\\Scripts\\";
//		script_path = file_path + sub_path + file_name + ".lua";
//		std::ofstream ofs;
//		ofs.open(script_path, std::ios::out);
//		if (ofs)
//		{
//			QDEBUG("created and added the script: {0}.lua , the path is {1}", script_name, script_path);
//		}
//		ofs << "--Update the script here\n"
//			   "function onUpdate()\n" 
//			   "end"<< std::endl;
//		ofs.close();
//		
//	}
//
//	void ScriptsSys::loadScript(const std::string& path/* ScriptComponent* comp*/)
//	{
//		script_component->L = std::make_shared<sol::protected_function_result>(lua_state->script_file(path));
//		//comp.L
//	}
//	
//	void ScriptsSys::reloadScript()
//	{
//		if (!script_path.empty())
//		{
//			loadScript(script_path);
//		}
//		else
//		{
//			loadScript(".\\Assets\\Scripts\\test.lua");
//		}
//	}
//
//
//
//	/**
//	* 
//	* void deleteAll(iter )
//	{
//		auto  vec = ecs->getXXXXBYComp<Lua>	();
//		for(auto lu : vec)
//		{
//			delete(lua);
//		}
//	}
//	
//	
//	*/
//
//	void ScriptsSys::deleteScript()
//	{
//		if (!script_path.empty())
//		{
//			if (std::remove(script_path.c_str()) == 0)
//			{
//				QDEBUG("Deleted the file: {0}.lua", script_name);
//			}
//		}
//		else
//		{
//			QWARN("No Script exists");
//		}
//
//	}
//	void ScriptsSys::importUpdate()
//	{
//		if (!is_imported)
//		{
//			script_component->update_function = (*lua_state)["onUpdate"];
//			is_imported = true;
//		}
//	}
//	std::string ScriptsSys::getScriptPath()
//	{
//		if (!script_path.empty())
//		{
//			return script_path;
//		}
//		QWARN("failed to get the script path");
//		return "";
//	}
//	std::string ScriptsSys::getScriptName()
//	{
//		if (!script_name.empty())
//		{
//			return script_name;
//		}
//		QWARN("failed to get the script name");
//		return "";
//	}
//}
//
////sol::lib::base,
////sol::lib::package,
////sol::lib::string,
////sol::lib::math,
////sol::lib::table