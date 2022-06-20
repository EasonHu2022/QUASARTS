#pragma once
#include "QuasartsEngine.h"
#include <string>
#include "GuiViews/GuiView.h"
#include "GuiViews/TextEditor.h"

class GameSceneView : public GuiView
{
private:

	ImGuiIO& io = ImGui::GetIO();

public:

	void on_add() override;
	void on_gui() override;
	void on_remove() override;
	void get_drag();

	ImGuizmo::OPERATION opType = ImGuizmo::OPERATION::TRANSLATE;
};
