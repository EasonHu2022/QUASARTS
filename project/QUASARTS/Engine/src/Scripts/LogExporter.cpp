#include "LogExporter.h"
#include "Logger/LogModule.h"


namespace Engine {

	void LogExporter::exportLog(sol::state& p_lua_state)
	{
		//for engine 
		p_lua_state.set_function("Qlog", std::function <void(const std::string&)>([](const std::string& str) {
			QDEBUG(str);
			}));

		p_lua_state.set_function("Qtrace", std::function <void(const std::string&)>([](const std::string& str) {
			QTRACE(str);
			}));

		p_lua_state.set_function("Qerror", std::function <void(const std::string&)>([](const std::string& str) {
			QERROR(str);
			}));

		//for game     
		p_lua_state.set_function("Glog", std::function <void(const std::string&)>([](const std::string& str) {
			DEBUG(str);
			}));

		p_lua_state.set_function("Gtrace", std::function <void(const std::string&)>([](const std::string& str) {
			TRACE(str);
			}));

		p_lua_state.set_function("Gerror", std::function <void(const std::string&)>([](const std::string& str) {
			ERROR(str);
			}));
	}
}