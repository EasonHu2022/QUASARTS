#include "MenuBarView.h"
#include <iostream>
// Decide OS
#include "HierarchyView.h"

void HierarchyView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void HierarchyView::on_gui()
{
	if (FileModule::Instance()->get_root() != NULL) {

		ImGui::SetNextWindowPos(ImVec2(0, 18));
		ImGui::SetNextWindowSize(ImVec2(window->get_width() * 0.15625, window->get_height() * 0.7));
		ImGui::Begin("Asset Hierarchy", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		auto models = Engine::Application::Instance->entityWorld->models;
		for (int i = 0; i < models.size(); i++) {

			if (ImGui::Selectable(models[i]->name.c_str())) {

				Engine::Application::Instance->miniecs->set_index(&i);
				
			}

		}
		ImGui::End();

	}
}

void HierarchyView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
