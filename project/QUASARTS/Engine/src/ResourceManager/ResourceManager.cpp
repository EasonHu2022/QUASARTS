#include "ResourceManager.h"

#include "ObjLoader.h"


namespace Engine
{
	ResourceManager* ResourceManager::instance = nullptr;
	/// <summary>
	/// Return a pointer to the instance of ResourceManager.
	/// </summary>
	/// <returns>Pointer to ResourceManager object.</returns>
	ResourceManager* ResourceManager::Instance()
	{
		if (nullptr == instance) instance = new ResourceManager();
		return instance;
	}


	ResourceManager::ResourceManager()
	{

		// Add loader.
		add_loader<ObjLoader>();

	} // ResourceManager()

	std::hash<std::string> ResourceManager::hString = std::hash<std::string>();


	void ResourceManager::init()
	{} // init()

	int ResourceManager::start()
	{
		return 0;
	} // start()

	void ResourceManager::update()
	{} // update()

	int ResourceManager::stop()
	{
		return 0;
	} // stop()
	
	void ResourceManager::release()
	{} // release()


	void ResourceManager::remove_resource(const size_t handle)
	{
		if (resMap.find(handle) == resMap.end())
		{
			QERROR("Could not find a resource with the given key: {0}", handle);
			return;
		}
		resMap.erase(handle);
	}


	bool ResourceManager::get_handle(const std::string& filepath, size_t* handle)
	{
		*handle = hString(filepath);
		if (resMap.find(*handle) == resMap.end())
			return false;
		return true;
	}

	bool ResourceManager::load_resource(const std::string& filepath, size_t* handle)
	{
		bool ret = false;

		// Check that filepath is not already associated with a resource.
		if (!get_handle(filepath, handle))
		{
			// Determine which loader supports the file extenion.
			std::string extension = getExtension(filepath);
			auto loader = loaders.find(extension);
			if (loader != loaders.end())
			{
				// Attempt to load the resource.
				ret = loader->second->load(filepath, *handle, resMap);
				if (!ret)
				{
					QERROR("Loader failed; resource could not be created from file: {0}", filepath);
				}
			}
			else
			{
				// Could not find a loader for the given extension.
				QERROR("File extension not supported: {0}", extension);
			}
		}
		else
		{
			// Resource already loaded.
			resMap[*handle]->refs++;
		}
		return ret;
		}
	}
