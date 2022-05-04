#include "QEditor.h"
#include "GuiViews/FileInputView.h"
#include "GuiViews/MenuBarView.h"
#include "GuiViews/GameSceneView.h"
#include "GuiViews/HierarchyView.h"
#include "GuiViews/AttributeView.h"
#include "GuiViews/TextEditorView.h"
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
	add_gui_view<TextEditorView>();
	//test_in_init();

	cameraController = new SceneCameraController();
}

void QEditor::on_update()
{
	Application::on_update();

	//for test
	test_in_update();

	// Handle relevant input.
	poll_input();

	cameraController->on_update();
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
	//if (Engine::Input::get_key_pressed(Q_KEY_W))
	//{
	//	QDEBUG("Get Key from Editor : W");
	//}

	//if (Engine::Input::get_key_released(Q_KEY_D))
	//{
	//	QDEBUG("Get Key from Editor : D");

	//}

	//if (Engine::Input::get_key_released(Q_KEY_A))
	//{
	//	QDEBUG("Get Key from Editor : A");
	//}

	//if (Engine::Input::get_key_released(Q_KEY_S))
	//{
	//	QDEBUG("Get Key from Editor : S");
	//}
}


void QEditor::poll_input()
{
	// N + L/R Control --> new scene
	if ( Engine::Input::get_key_combination( { Q_KEY_N, Q_KEY_LEFT_CONTROL } ) )
	{
		getGuiView<MenuBarView>()->new_scene = true;
	}

	if (Engine::Input::get_key_combination({ Q_KEY_O, Q_KEY_LEFT_CONTROL }))
	{
		std::string proj_file = getGuiView<MenuBarView>()->OpenFileDialogue();
		if (proj_file.compare("N/A") != 0)
			FileModule::Instance()->open_root(proj_file);
		
	}

	if (Engine::Input::get_key_combination({ Q_KEY_G, Q_KEY_LEFT_SHIFT }))
	{
		QDEBUG("Get Key from Editor : G");
		//ImGui::SetWindowFocus("Script Editor");
	}

	if (Engine::Input::get_key_combination({ Q_KEY_D, Q_KEY_LEFT_CONTROL }))
	{
		Engine::ECSManager::Instance()->destroy_entity(Engine::ECSManager::Instance()->get_current_entity());
		Engine::ECSManager::Instance()->set_current_entity(TOO_MANY_ENTITIES);
	}
}