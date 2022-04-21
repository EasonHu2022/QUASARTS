#pragma once
#include "Core/IModule.h"
#include "Core/Core.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "IResource.h"
#include "Logger/LogModule.h"
namespace Engine
{
	class ResourceManager
	{
	private:
		static ResourceManager* instance;
		std::unordered_map<size_t, std::shared_ptr<IResource>> resMap;
	public:
		static ResourceManager* Instance();

		template<class T>
		size_t add_resource(std::shared_ptr<T> res)
		{
			
			std::hash<std::string> h;
			size_t handle = h(res.get()->path);
			if (resMap.find(handle) == resMap.end())
			{
				resMap.emplace(handle, res);
			}		
			else
			{
				QERROR("Load the same resource at  path : {0}, please check", res.get()->path);
			}
			return handle;
		}
		
		template<class T>
		auto get_resource(size_t handle)
		{
			if (resMap.find(handle) == resMap.end())
			{
				QERROR("resource at  path : {0}  hasn't been loaded, please check", handle);
				std::shared_ptr<T> nul;
				return nul;
			}
			return std::static_pointer_cast<T>(resMap[handle]);
		}

		void remove_resource(size_t handle)
		{
			if (resMap.find(handle) == resMap.end())
			{
				QERROR("resource at  path : {0}  doesn't exist, please check", handle);
				return ;
			}
			resMap.erase(handle);
		}
		
	
	};
}
