#include "FileLoader.h"
#include "ResourceManager/ResourceManager.h"
#include "Logger/LogModule.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
namespace Engine
{
	size_t FileLoader::load(const std::string& obj)
	{
		ResourceManager* res = ResourceManager::Instance();

		std::hash<std::string> h;
		size_t handle = h(obj);

		auto exist = res->get_resource<FileResource>(handle);
		if (exist.get() == NULL)
		{
			auto fileResource = std::make_shared<FileResource>();
			fileResource.get()->path = obj;
			fileResource.get()->refs = 1;
		
			//read
			std::ifstream files;
			files.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                // open files
                files.open(obj);
                std::stringstream txtStream;
                // read file's buffer contents into streams
                txtStream << files.rdbuf();
               
                // close file handlers
                files.close();

                // convert stream into string
                fileResource.get()->textContent = txtStream.str();


            }
            catch (std::ifstream::failure& e)
            {
				QERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: {0}",e.what());
            }

			return res->add_resource<FileResource>(fileResource);
		}

		exist.get()->refs++;

		return handle;
	}
};