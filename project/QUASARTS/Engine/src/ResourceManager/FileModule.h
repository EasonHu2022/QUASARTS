#pragma once
#include "Core/IModule.h"
#include "Core/Core.h"
#include <vector>
#include <string>
class QS_API FileModule : public IModule
{
private:
	static FileModule* instance;
public:
	static FileModule* Instance();

	/// <summary>
/// init the FileModule
/// </summary>
	void init() override;


	/// <summary>
	/// start the FileModule
	/// </summary>
	/// <returns></returns>
	int start() override;

	/// <summary>
	/// update
	/// </summary>
	void update() override;

	/// <summary>
	/// stop the FileModule 
	/// </summary>
	/// <returns></returns>
	int stop() override;

	/// <summary>
	/// release theFileModule
	/// </summary>
	void release() override;

//functions
private:




public:
	struct QFileNode
	{
		char* name;
		//abs path
		char* path;
		//type
		//todo
	};
	struct QDirectoriesNode
	{
		char* name;
		//abs path
		char* path;
		//list of children
		std::vector<QDirectoriesNode*> children;
		//list of file
		std::vector<QFileNode*> files;
		
	};
	
private:
	QDirectoriesNode* cur_root = nullptr;
	const char* cur_workdir = NULL;
	const char* cmd = "mkdir ";
	//default add 2 folders(Resources and Scripts) under Assets
	const char*  folders[5] = {"\\Assets","\\Release","\\ProjectSetting","\\Assets\\Scripts","\\Assets\\Resources"};
	
public:
	/// <summary>
	/// Create the whole working dir
	/// </summary>
	/// <param name="p">path</param>
	/// <param name="projectName">project folder's name</param>
	/// <returns></returns>
	int create_workdir(const char* p, const char* projectName);

	/// <summary>
	/// create resouce node from cur work dir
	/// </summary>
	int create_resource_node();
	int recursively_build_dirnode(QDirectoriesNode* node);

	/// <summary>
	/// update resouce node from cur work dir
	/// </summary>
	int update_resource_node();


	/// <summary>
	/// get the root node of directories
	/// </summary>
	/// <returns>GDirectoriesNode root nood</returns>
	const QDirectoriesNode* get_root();

	/// <summary>
	/// build a db for files 
	/// now just for editor
	/// </summary>
	void build_file_vdb(std::string root_dir);
};