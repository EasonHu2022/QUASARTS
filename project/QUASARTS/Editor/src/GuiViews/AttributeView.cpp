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
		
			if (Engine::Application::Instance->miniecs->index > -1) {

				if (i != Engine::Application::Instance->miniecs->index) {

					i = Engine::Application::Instance->miniecs->index;
					
					vec3f[0] = Engine::Application::Instance->miniecs->entities[i]->attributes[0].x;
					vec3f[1] = Engine::Application::Instance->miniecs->entities[i]->attributes[0].y;
					vec3f[2] = Engine::Application::Instance->miniecs->entities[i]->attributes[0].z;
					vec3f2[0] = Engine::Application::Instance->miniecs->entities[i]->attributes[1].x;
					vec3f2[1] = Engine::Application::Instance->miniecs->entities[i]->attributes[1].y;
					vec3f2[2] = Engine::Application::Instance->miniecs->entities[i]->attributes[1].z;

				}

				ImGui::InputFloat3("Position", vec3f);
				Engine::Application::Instance->miniecs->entities[i]->attributes[0].x = vec3f[0];
				Engine::Application::Instance->miniecs->entities[i]->attributes[0].y = vec3f[1];
				Engine::Application::Instance->miniecs->entities[i]->attributes[0].z = vec3f[2];

				ImGui::Separator();


				ImGui::InputFloat3("Orientation", vec3f2);
				Engine::Application::Instance->miniecs->entities[i]->attributes[1].x = vec3f2[0];
				Engine::Application::Instance->miniecs->entities[i]->attributes[1].y = vec3f2[1];
				Engine::Application::Instance->miniecs->entities[i]->attributes[1].z = vec3f2[2];
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
