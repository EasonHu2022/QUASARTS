#include "MenuBarView.h"
#include <iostream>
// Decide OS
#include "AttributeView.h"

void AttributeView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void AttributeView::on_gui()
{
	if (FileModule::Instance()->get_root() != NULL) {

		ImGui::SetNextWindowPos(ImVec2(window->get_width() * 0.85625+18, 18));
		ImGui::SetNextWindowSize(ImVec2(window->get_width() * 0.34375, window->get_height() * 1.0922222222+41));
		ImGui::Begin("Attribute Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		
		if (!game) {
		
			if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES && Engine::ECSManager::Instance()->has_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH)) {
			
				/**/
				show_mesh();
				show_transform();
			}
		
		}
		if (ImGui::Button("Game")) {
			game = !game;
		}
		if (ImGui::Button("Print")) {
			std::cout << Engine::Application::Instance->miniecs->entities[Engine::Application::Instance->miniecs->index]->attributes[0].x << std::endl;
		}
		ImGui::End();

	}
}

void AttributeView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}

void AttributeView::show_mesh() {
	Engine::MeshComponent* mesh = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH);
	std::string obj_name = "Name of mesh file: " + mesh->path.substr(mesh->path.find_last_of("\\") + 1);
	ImGui::Separator();
	ImGui::Text(obj_name.c_str());
}

void AttributeView::show_transform() {
	ImGui::Separator();
	Engine::TransformComponent* transform = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM);
	static float pos[3] = { transform->position.x ,transform->position.y, transform->position.z };
	static float rot[3] = { transform->rotation.x ,transform->rotation.y, transform->rotation.z };
	static float scal[3] = { transform->scale.x ,transform->scale.y, transform->scale.z };

	ImGui::InputFloat3(" Position", pos);
	transform->position.x = pos[0];
	transform->position.y = pos[1];
	transform->position.z = pos[2];
	Engine::ECSManager::Instance()->replace_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM, *transform);

	ImGui::InputFloat3(" Rotation", rot);
	transform->rotation.x = rot[0];
	transform->rotation.y = rot[1];
	transform->rotation.z = rot[2];
	Engine::ECSManager::Instance()->replace_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM, *transform);

	ImGui::InputFloat3(" Scale", scal);
	transform->scale.x = scal[0];
	transform->scale.y = scal[1];
	transform->scale.z = scal[2];
	Engine::ECSManager::Instance()->replace_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM, *transform);
	
}