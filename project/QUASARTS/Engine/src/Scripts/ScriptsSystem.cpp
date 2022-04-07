#include "ScriptsSystem.h"
#include "Logger/LogModule.h"

#include <fstream>
#include "Utilities/FileParser.h"

namespace Engine {

	// singleton
	ScriptsSys* ScriptsSys::instance = nullptr;
	
	/// <summary>
	/// Create a singleton of the .
	/// </summary>
	/// <returns> The unique instance of the Scripts system. </returns>
	ScriptsSys* ScriptsSys::Instance()
	{
		if (nullptr == instance)
			instance = new ScriptsSys();
		return instance;
	}
		
	/// <summary>
	/// init
	/// </summary>
	void ScriptsSys::init()
	{
		//lib 
		lua.open_libraries(sol::lib::base);

		//export
		exportLog();
	}

	/// <summary>
	/// start
	/// </summary>
	/// <returns>res</returns>
	int ScriptsSys::start()
	{
		return 0;
	}

	/// <summary>
	/// update
	/// </summary>
	void ScriptsSys::update()
	{

	}

	/// <summary>
	/// stop
	/// </summary>
	/// <returns>res</returns>
	int ScriptsSys::stop()
	{
		return 0;
	}

	/// <summary>
	/// release
	/// </summary>
	void ScriptsSys::release()
	{

	}
	
	void ScriptsSys::createScript(const std::string& file_name)
	{	
		script_name = file_name;
		std::ofstream ofs;
		ofs.open(root + file_name + ".lua", std::ios::out);
		if (ofs)
		{
			QDEBUG("Created a {0} script.", script_name);
		}
		ofs << "--Scripts test: we now can use our log function" << std::endl;
		ofs << "--And we have a simple entity class here. usage: printPos, setX, setY" << std::endl;
		ofs.close();
		
	}

	void ScriptsSys::loadScript(const std::string& path)
	{
		lua.script_file(path);
	}
	
	void ScriptsSys::exportLog()
	{
		//for engine 
		lua.set_function("Qlog", std::function <void(const std::string&)>([](const std::string& str) {
			QDEBUG(str);
			}));

		lua.set_function("Qtrace", std::function <void(const std::string&)>([](const std::string& str) {
			QTRACE(str);
			}));

		lua.set_function("Qerror", std::function <void(const std::string&)>([](const std::string& str) {
			QERROR(str);
			}));

		//for game     
		lua.set_function("Glog", std::function <void(const std::string&)>([](const std::string& str) {
			DEBUG(str);
			}));

		lua.set_function("Gtrace", std::function <void(const std::string&)>([](const std::string& str) {
			TRACE(str);
			}));

		lua.set_function("Gerror", std::function <void(const std::string&)>([](const std::string& str) {
			ERROR(str);
			}));
	}

	void ScriptsSys::updateScript()
	{
		loadScript(root + script_name + ".lua");
	}

	void ScriptsSys::deleteScript()
	{
		std::string file_path = root + script_name + ".lua";

		if (std::remove(file_path.c_str()) == 0)
		{
			QDEBUG("Deleted the file: {0}.lua", script_name);
		}

	}

}

//sol::lib::base,
//sol::lib::package,
//sol::lib::string,
//sol::lib::math,
//sol::lib::table