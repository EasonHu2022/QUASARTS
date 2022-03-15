#pragma once
#include "IModule.h"
#include <vector>
class FileModule:public IModule
{
public:
	enum QLOADTYPE
	{
		QASYNC = 0,
		QSYNC,
	};
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

	/// <summary>
	/// Synchronous File I/O
	/// </summary>
	/// <returns>file handle(unique)</returns>
	int sync_read_file();
	/// <summary>
	/// Asynchronous File I/O
	/// </summary>
	/// <returns>file handle</returns>
	int async_read_file();


public:
	struct QFileNode
	{
		char* name;
		//path relative to root
		char* path;
		//type
		//todo
	};
	struct QDirectoriesNode
	{
		char* name;
		//path relative to root
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
	/// <param name="p"> path </param>
	/// <returns></returns>
	int create_workdir(const char* p);

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
	/// Load files from a fixed dir called Resources
	/// </summary>
	/// <param name="name">file path relative to the resource folder</param>
	/// <param name="_type">load type, sync or async</param>
	/// <returns>file handle(generated uid maybe)</returns>
	int load_resources(char* name, QLOADTYPE _type = QSYNC);

	/// <summary>
	/// release the loaded resources by handle
	/// </summary>
	/// <param name="handle">file handle</param>
	void release_resource(int handle);
};