#include "QEditor.h"
#include "GuiViews/FileInputView.h"
#include "GuiViews/MenuBarView.h"
#include "Core/Mesh.h"

QEditor::QEditor()
{
}

QEditor::~QEditor()
{
}

void QEditor::init()
{
	Application::init();
	/*
	* add menu bar
	*/
	add_gui_view<MenuBarView>();
	add_gui_view<FileInputView>();
	test_in_init();
}

void QEditor::on_update()
{
	Application::on_update();

	//for test
	test_in_update();
}

void QEditor::on_gui()
{
	for (auto& v : guiViews)
	{
		v.second->on_gui();
	}

	/*
		base fun
	*/

	Application::on_gui();
}

/// <summary>
/// place for all test code
/// </summary>
void QEditor::test_in_init()
{
	std::string path = "F:\\WorkSpace\\LEEDS\\Graphics and Render\\Assignment2\\objects\\cube_flat.obj";
	std::unordered_map<std::string, std::shared_ptr<Engine::Mesh>> meshes{};
	loaderFactory->load(path, meshes);
	QDEBUG("test load meshes");

}

void QEditor::test_in_update()
{
}
