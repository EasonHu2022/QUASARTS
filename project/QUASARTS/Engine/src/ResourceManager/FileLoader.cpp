#include "FileLoader.h"
#include "Logger/LogModule.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
namespace Engine
{
	bool FileLoader::load(const std::string& filepath, const size_t handle, std::unordered_map<size_t, std::shared_ptr<IResource>>& resMap)
	{
		
		//read
		std::ifstream files;
		files.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        auto fileResource = std::make_shared<FileResource>();

        std::string shortName = filepath.substr(filepath.find_last_of("/\\") + 1, filepath.find_last_of("."));
        std::string extension = filepath.substr(filepath.find_last_of(".") + 1);

        fileResource->fileShortname = shortName;
        fileResource->extension = extension;

        try
        {
            // open files
            files.open(filepath);
            std::stringstream txtStream;
            // read file's buffer contents into streams
            txtStream << files.rdbuf();
               
            // close file handlers
            files.close();

            // convert stream into string
            fileResource->textContent = txtStream.str();


        }
        catch (std::ifstream::failure& e)
        {
			QERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: {0}", e.what());
            return false;
        }

		resMap.emplace(handle, fileResource);
		return true;
	}
};