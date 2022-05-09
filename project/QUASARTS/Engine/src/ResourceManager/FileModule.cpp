#include "FileModule.h"
#include "Logger/LogModule.h"
#include "ECS/System/AudioSystem.h"
#include <stdlib.h>
#include <iostream>
#include "Core/QUtil.h"
#include <cstring>

#ifdef QS_WINDOWS
	#include <io.h>
	#include "atlstr.h"
#endif

FileModule* FileModule::instance = nullptr;
/// <summary>
/// return an instance
/// </summary>
/// <returns></returns>
FileModule* FileModule::Instance()
{
	if (nullptr == instance)
	{
		instance = new FileModule();
	}
		
		return instance;
	
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
	//const char* wdir = "F:\\WorkSpace";
	//const char* projName = "TestProject";
	//create_workdir(wdir, projName);
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

void FileModule::build_file_vdb(std::string root_dir)
{
}

#ifdef QS_WINDOWS
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
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
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
		delete[] path;
		delete[] next;
		return 0;
	}
#else
	int FileModule::recursively_build_dirnode(QDirectoriesNode* node)
	{
		return 0;
	}
#endif

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

int FileModule::create_workdir(const char* p, const char* projectName)
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

void FileModule::open_root(std::string root) {

	std::ifstream read(root);
	getline(read, current_root);
	cur_root = new QDirectoriesNode();
	cur_root->path = (char*)current_root.c_str(); 
	cur_root->name = "Assets"; 
	read.close();
}

void FileModule::save_root(std::string root, std::string name) {

#if defined(_WIN32)
	std::string project_file = root + "\\" + name + "\\" + name + ".q";
#else
	std::string project_file = root + "/" + name + "/" + name + ".q";
#endif
	std::ofstream of(project_file);
	if (of.is_open())
	{
		of << cur_root->path << std::endl;
		QDEBUG("Project Saved");
	}
	else
	{
		QDEBUG("Failed to save project");
		
	}

	of.close();
}

std::string FileModule::get_internal_assets_path()
{

	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	
	for (int i = 0; i < 4; i++)
	{
		int pos = path.ReverseFind('\\');
		path = path.Left(pos);		
	}
	auto str = WChar2Ansi(path.GetBuffer());
	str += "\\Assets\\";
	return str;
}

