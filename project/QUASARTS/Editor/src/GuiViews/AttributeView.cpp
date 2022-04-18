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

		ImGui::SetNextWindowPos(ImVec2(window->get_width() * 0.85625, 18));
		ImGui::SetNextWindowSize(ImVec2(window->get_width() * 0.34375, window->get_height() * 1.0922222222));
		ImGui::Begin("Attribute Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		
		if (!game) {
		
			if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES && Engine::ECSManager::Instance()->has_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH)) {
			
				Engine::MeshComponent mesh = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH);
				static float v1[3] = { mesh.one,mesh.two,mesh.three };
				ImGui::InputFloat3("Position", v1);
				if (ImGui::Button("change")) {
					mesh.one = v1[0];
					mesh.two = v1[1];
					mesh.three = v1[2];
					Engine::ECSManager::Instance()->replace_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH, mesh);
					Engine::MeshComponent mesh2 = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH);
					std::cout << mesh2.one <<std::endl;
				}
				
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
