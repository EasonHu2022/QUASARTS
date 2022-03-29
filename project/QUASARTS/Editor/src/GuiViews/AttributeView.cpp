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
		ImGui::End();

	}
}

void AttributeView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
