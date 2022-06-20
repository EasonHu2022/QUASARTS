#include "QEditor.h"
#include "GuiViews/FileInputView.h"
#include "GuiViews/MenuBarView.h"
#include "GuiViews/GameSceneView.h"
#include "GuiViews/HierarchyView.h"
#include "GuiViews/AttributeView.h"
#include "Core/Mesh.h"
#include "Core/Input.h"

struct comp
{
	template<typename T>
	bool operator()(const T& lhs, const T& rhs) const
	{
		if (lhs.texName.compare(rhs.texName) == 0)
			return true;
		else
			return false;

	}
};

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

	if (Engine::Input::get_key_combination({ Q_KEY_G, Q_KEY_LEFT_SHIFT }))
	{
		QDEBUG("Get Key from Editor : G");
		//ImGui::SetWindowFocus("Script Editor");
	}

	if (Engine::Input::get_key(Q_KEY_LEFT_CONTROL))
	{

		if (Engine::Input::get_key_pressed(Q_KEY_O))
		{
			std::string proj_file;
#if defined(_WIN32)
			proj_file = getGuiView<MenuBarView>()->OpenFileDialogue(L"All Files (*.*)\0*.q\0");
#else
			proj_file = getGuiView<MenuBarView>()->OpenFileDialogue("\"\"*.q\"\"");
#endif
			if (proj_file.compare("N/A") != 0)
				FileModule::Instance()->open_root(proj_file);
		}

	}

	if (Engine::Input::get_key(Q_KEY_LEFT_CONTROL))
	{
		if (Engine::Input::get_key_pressed(Q_KEY_N))
		{
			getGuiView<MenuBarView>()->new_scene = true;
		}
	}

	if (Engine::Input::get_key(Q_KEY_LEFT_CONTROL))
	{
		if (Engine::Input::get_key_pressed(Q_KEY_D))
		{
			if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES) {
				Engine::ECSManager::Instance()->destroy_entity(Engine::ECSManager::Instance()->get_current_entity());
				Engine::ECSManager::Instance()->set_current_entity(TOO_MANY_ENTITIES);
			}
		}
	}

	if (Engine::Input::get_key(Q_KEY_LEFT_SHIFT))
	{
		if (Engine::Input::get_key_pressed(Q_KEY_D))
		{
			Engine::ParticleTexture tex1, tex2;
			Engine::ParticleTexture tex3;
			std::pair<Engine::ParticleTexture, std::vector<Engine::Particle>> pair1;
			std::pair<Engine::ParticleTexture, std::vector<Engine::Particle>> pair2;
			std::vector<Engine::Particle> particles;
			std::vector<Engine::Particle> particles2;
			pair1.first = tex1;
			pair1.second = particles;
			pair2.first = tex2;
			pair2.second = particles2;
			std::map<std::string, std::pair<Engine::ParticleTexture, std::vector<Engine::Particle>>> emitters;
			emitters.insert(std::make_pair("the first one", pair1));
			emitters.insert(std::make_pair("the second1 one", pair2));
			//emitters.insert(std::make_pair(tex2, particles2));
			//emitters.insert(std::make_pair(tex3, particles2));
			for (auto& [key, value] : emitters) {
				//std::cout << key << std::endl;
			}
			if (emitters.count("the first sone")) { std::cout << "we got it" << std::endl; }
		}
	}

	if (Engine::Input::get_key(Q_KEY_LEFT_CONTROL))
	{
		auto gameScene = getGuiView<GameSceneView>();
		if (Engine::Input::get_key_pressed(Q_KEY_W))
		{
			gameScene->opType = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (Engine::Input::get_key_pressed(Q_KEY_E))
		{
			gameScene->opType = ImGuizmo::OPERATION::ROTATE;
		}
		if (Engine::Input::get_key_pressed(Q_KEY_R))
		{
			gameScene->opType = ImGuizmo::OPERATION::SCALE;
		}
	}
	
}