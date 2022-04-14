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
		//auto models = Engine::Application::Instance->entityWorld->models;
		std::vector<unsigned int> entities = Engine::ECSManager::Instance()->get_entity_ID_match();
		for (int i = 0; i < entities.size(); i++) {

			if (!Engine::ECSManager::Instance()->has_parent(entities[i])) {
				build_children(entities[i], true);
			}

		}
		ImGui::End();

	}
}

void HierarchyView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}

void HierarchyView::build_children(unsigned int entityID, bool show) {

	if (Engine::ECSManager::Instance()->has_children(entityID)) {

		std::set<unsigned int> children = Engine::ECSManager::Instance()->get_children(entityID);
		bool is_expanded = ImGui::TreeNodeEx((void*)entityID, ImGuiTreeNodeFlags_FramePadding, "", nullptr);
		ImGui::SameLine();
		if (ImGui::Selectable(Engine::ECSManager::Instance()->get_entityName(entityID).c_str(), ImGuiTreeNodeFlags_Selected)) {

			Engine::ECSManager::Instance()->set_current_entity(entityID);

		}
		if (is_expanded) {
			for (auto child : children)
				build_children(child, is_expanded);
			ImGui::TreePop();
		}

	}
	else {
	
		if (show) {
		
			if (ImGui::Selectable(Engine::ECSManager::Instance()->get_entityName(entityID).c_str())) {

				Engine::ECSManager::Instance()->set_current_entity(entityID);

			}
		
		}
	
	}
}