#include "QEditor.h"
#include "GuiViews/FileInputView.h"
#include "GuiViews/MenuBarView.h"
#include "GuiViews/GameSceneView.h"
#include "GuiViews/HierarchyView.h"
#include "GuiViews/AttributeView.h"
#include "Core/Mesh.h"
#include "Core/Input.h"

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
	add_gui_view<GameSceneView>();
	add_gui_view<HierarchyView>();
	add_gui_view<AttributeView>();
	//test_in_init();
}

void QEditor::on_update()
{
	Application::on_update();

	//for test
	test_in_update();

	// Handle relevant input.
	poll_input();
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
/*void QEditor::test_in_init()
{
	std::string path = "F:\\WorkSpace\\LEEDS\\Graphics and Render\\Assignment2\\objects\\cube_flat.obj";
	std::unordered_map<std::string, std::shared_ptr<Engine::Mesh>> meshes{};
	loaderFactory->load(path, meshes);
	auto model = new Engine::ModelResource();
	model->meshes = meshes;
	model->name = "name";
	Engine::Application::entityWorld->add_entity(model);

	auto models = Engine::Application::entityWorld->models;
	for (auto m : models) {
	
		auto name = m->name;
	
	}
	QDEBUG("test load meshes");

}*/

void QEditor::test_in_update()
{
	if (Engine::Input::get_key_pressed(Q_KEY_W))
	{
		QDEBUG("Get Key from Editor : W");
	}

	if (Engine::Input::get_key_released(Q_KEY_W))
	{
		QDEBUG("Get Key from Editor : W");
	}
}


void QEditor::poll_input()
{
	// N + L/R Control --> new scene
	if ( Engine::Input::get_key_combination( { Q_KEY_N, Q_KEY_LEFT_CONTROL } ) )
	{
		getGuiView<MenuBarView>()->new_scene = true;
	}
}