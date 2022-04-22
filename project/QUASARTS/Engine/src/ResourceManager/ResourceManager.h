#pragma once

#include "Core/Core.h"
#include "Core/IManager.h"
#include "Core/IResource.h"
#include "Core/ILoader.h"
#include "Core/QUtil.h"
#include "Logger/LogModule.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

namespace Engine
{
	class QS_API ResourceManager : public IManager
	{
	private:
		static ResourceManager* instance;
		ResourceManager();
	public:
		static ResourceManager* Instance();
		~ResourceManager() {}

		void init();
		int start();
		void update();
		int stop();
		void release();


	private:
		static std::hash<std::string> hString;
		std::unordered_map<size_t, std::shared_ptr<IResource>> resMap;
		std::unordered_map<std::string, std::shared_ptr<ILoader>> loaders;
		//std::unordered_set<std::string> supportedExtensions;

	public:
		template<class T>
		std::shared_ptr<T> get_resource(const size_t handle)
		{
			if (resMap.find(handle) == resMap.end())
			{
				QERROR("Could not find a resource with the given key: {0}", handle);
				return std::shared_ptr<T>();
			}
			return std::static_pointer_cast<T>(resMap[handle]);
		}

		void remove_resource(const size_t handle);

		/// <summary>
		/// Computes a handle from the given filepath, stores it at the address given by the pointer parameter, and checks if it is already in the resource map.
		/// </summary>
		/// <param name="filepath">Filepath of a resource data file.</param>
		/// <param name="handle">Pointer to a variable which will store the computed resource handle.</param>
		/// <returns>True if the resource at the given filepath is already loaded, false otherwise.</returns>
		bool get_handle(const std::string& filepath, size_t* handle);

		bool load_resource(const std::string& filepath, size_t* handle);

	private:
		template<typename T>
		inline void add_loader()
		{
			auto loader = std::make_shared<T>();
			for (auto ext : T::EXTENSIONS)
			{
				loaders.emplace(ext, loader);
				//supportedExtensions.emplace(ext);
			}
		}
		
	
	};
}
