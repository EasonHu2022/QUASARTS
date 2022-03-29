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
		std::vector<float> v1;
		std::vector<float> v2;
		float vec3f[3];
		if (Engine::Application::Instance->miniecs->index > -1) {

			int i = Engine::Application::Instance->miniecs->index;
			if (v1.empty()) {
			
				v1.push_back(Engine::Application::Instance->miniecs->entities[i]->attributes[0].x);
				v1.push_back(Engine::Application::Instance->miniecs->entities[i]->attributes[0].y);
				v1.push_back(Engine::Application::Instance->miniecs->entities[i]->attributes[0].z);

				v2.push_back(Engine::Application::Instance->miniecs->entities[i]->attributes[1].x);
				v2.push_back(Engine::Application::Instance->miniecs->entities[i]->attributes[1].y);
				v2.push_back(Engine::Application::Instance->miniecs->entities[i]->attributes[1].z);

				vec3f[0] = v1[0];
				vec3f[1] = v1[1];
				vec3f[2] = v1[2];
				vec3f2[0] = v2[0];
				vec3f2[1] = v2[1];
				vec3f2[2] = v2[2];
			
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
		ImGui::End();

	}
}

void AttributeView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
