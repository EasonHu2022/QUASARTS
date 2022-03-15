#include "FileModule.h"
#include "LogModule.h"
#include <stdlib.h>
#include <iostream>
#include <io.h>
#include "QUtil.h"

FileModule* FileModule::instance = nullptr;
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
FileModule* FileModule::Instance()
{
	if (nullptr == instance)
		return new FileModule();
	else
	{
		return instance;
	}
}


#pragma region LifeSpan
void FileModule::init()
{

}


/// <summary>
/// start the FileModule
/// </summary>
/// <returns></returns>
int FileModule::start()
{
	//test
	//test here
	//const char* workdir = "F:\\WorkSpace\\TestProject";
	//create_workdir(workdir);
	return 0;
}

/// <summary>
/// update
/// </summary>
void FileModule::update()
{
}

/// <summary>
/// stop the FileModule 
/// </summary>
/// <returns></returns>
int FileModule::stop()
{
	return 0;
}

/// <summary>
/// release theFileModule
/// </summary>
void FileModule::release()
{
}
#pragma endregion



const FileModule::QDirectoriesNode* FileModule::get_root()
{
	return cur_root;
}


int FileModule::recursively_build_dirnode(QDirectoriesNode* node)
{
	intptr_t  handle = 0;
	struct _finddata_t fileinfo;
	const char* suffix = "\\* ";
	const char* suffDir = "\\";
	char* path = char_merge(node->path, suffix);
	char* next = char_merge(node->path, suffDir);
	
	handle = _findfirst(path, &fileinfo);
	if (handle != -1)
	{
		do
		{
			if (fileinfo.attrib & _A_SUBDIR)
			{
				//it's a subfolder
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//this if is for linux
				{
					//create new node
					auto dir_node = new QDirectoriesNode();
					dir_node->name = fileinfo.name;
					char* node_path = char_merge(next, fileinfo.name);
					dir_node->path = node_path;
					node->children.push_back(dir_node);
					QDEBUG("create a dirnode under {0}. path is {1}",node->path, dir_node->path);
					recursively_build_dirnode(dir_node);
				}
			}
			else
			{
				//files
				auto file_node = new QFileNode();
				file_node->name = fileinfo.name;
				char* node_path = char_merge(next, fileinfo.name);
				file_node->path = node_path;
				QDEBUG("create a filenode under {0}. name is {1}", node->path, file_node->path);
				node->files.push_back(file_node);
			}
		} while (_findnext(handle, &fileinfo) == 0);
		_findclose(handle);
	}

	return 0;
}

int FileModule::create_resource_node()
{	
	cur_root = new QDirectoriesNode();
	char* asset_dir = char_merge(cur_workdir,folders[0]);
	cur_root->path = asset_dir;
	cur_root->name = "Assets";
	return recursively_build_dirnode(cur_root);
}

int FileModule::update_resource_node()
{
	if (NULL == cur_root)
	{
		QDEBUG(" Find Root node is null, create new node");
		create_resource_node();
	}
	return 0;

}

int FileModule::create_workdir(const char* p)
{
	cur_workdir = p;
	if (0 == access(p, 0))
	{
		QERROR("Directory exists in {0}", p);
		update_resource_node();
		return 0;
		//return 1;
	}



	char* mkCmd = char_merge(cmd,p);

	QDEBUG("create dir at {0}", mkCmd);
	system(mkCmd);

	for (const char* f : folders)
	{
		
		char* fcmd = char_merge(mkCmd, f);
		QDEBUG("create folders at {0}", fcmd);
		system(fcmd);
	}



	update_resource_node();

	return 0;
}
