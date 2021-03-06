#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/FileInputView.h"

class MenuBarView : public GuiView
{
public:
	bool new_project;
	bool new_scene;
	bool new_script;
	bool new_entity;
	bool new_child;
	bool new_attribute;
	std::string folder_path;
	std::string project_name;
	#ifdef QS_WINDOWS
		static std::string OpenFileDialogue(const wchar_t*);
	#else
		static std::string OpenFileDialogue(std::string);
	#endif
	static std::string OpenFolderDialogue();
	void load_object(std::string, std::string);
	void newProject();
	void newScene();
	void newScript();
	void newEntity();
	void newChild();
	void newAttribute(const int componentType);
	void createMaterial(unsigned int entityID);
	void createMesh(unsigned int entityID);
public:
	void on_add() override;
	void on_gui() override;
	void on_remove() override;
};
