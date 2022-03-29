#include "FileParser.h"



namespace Engine {

	std::string getFileName(const std::string& file_path)
	{
		//two possible ways
		int pos1 = file_path.find_last_of("/");  //find file path
		int pos2 = file_path.find_last_of("\\"); //find file path

		if (pos1 != std::string::npos)
		{
			int dot_pos = file_path.rfind(".");  //remove suffix
			std::string file_name = file_path.substr(pos1 + 1, dot_pos - pos1 - 1);   //get object name 
			return file_name;
		}

		else if (pos2 != std::string::npos)
		{
			int dot_pos = file_path.rfind(".");  //remove suffix
				std::string file_name = file_path.substr(pos2 + 1, dot_pos - pos2 - 1);   //get object name 
				return file_name;
		}

		return file_path;
	}


	//Todo

}