#include "FileManager.h"
#include "LogModule.h"
#include <stdlib.h>
#include <iostream>
#include <io.h>
#include "QUtil.h"

FileManager* FileManager::instance = nullptr;
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
FileManager* FileManager::Instance()
{
	if (nullptr == instance)
		return new FileManager();
	else
	{
		return instance;
	}
}


#pragma region LifeSpan
//void FileManager::init()
//{
//
//}
//
//
///// <summary>
///// start the FileModule
///// </summary>
///// <returns></returns>
//int FileManager::start()
//{
//	//test
//	//test here
//	//const char* wdir = "F:\\WorkSpace";
//	//const char* projName = "TestProject";
//	//create_workdir(wdir, projName);
//	return 0;
//}
//
///// <summary>
///// update
///// </summary>
//void FileManager::update()
//{
//}
//
///// <summary>
///// stop the FileModule 
///// </summary>
///// <returns></returns>
//int FileManager::stop()
//{
//	return 0;
//}
//
///// <summary>
///// release theFileModule
///// </summary>
//void FileManager::release()
//{
//}
#pragma endregion



const FileManager::QDirectoriesNode* FileManager::get_root()
{
	return cur_root;
}


int FileManager::recursively_build_dirnode(QDirectoriesNode* node)
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
					QDEBUG("create a dirnode under {0}. path is {1}", node->path, dir_node->path);
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
	delete[] path;
	delete[] next;
	return 0;
}

int FileManager::create_resource_node()
{
	cur_root = new QDirectoriesNode();
	char* asset_dir = char_merge(cur_workdir, folders[0]);
	cur_root->path = asset_dir;
	cur_root->name = "Assets";
	return recursively_build_dirnode(cur_root);
}

int FileManager::update_resource_node()
{
	if (NULL == cur_root)
	{
		QDEBUG(" Find Root node is null, create new node");
		create_resource_node();
	}
	return 0;

}

int FileManager::create_workdir(const char* p, const char* projectName)
{
	const char* sig = "\\";
	cur_workdir = char_merge(p, sig);
	cur_workdir = char_merge(cur_workdir, projectName);
	if (0 == access(cur_workdir, 0))
	{
		QERROR("Directory exists in {0}", cur_workdir);
		update_resource_node();
		return 0;
		//return 1;
	}



	char* mkCmd = char_merge(cmd, cur_workdir);

	QDEBUG("create dir at {0}", mkCmd);
	system(mkCmd);

	for (const char* f : folders)
	{

		char* fcmd = char_merge(mkCmd, f);
		QDEBUG("create folders at {0}", fcmd);
		system(fcmd);
		delete[] fcmd;
	}



	update_resource_node();


	delete[] mkCmd;
	return 0;
}

