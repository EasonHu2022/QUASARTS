#include "MenuBarView.h"
#include <iostream>
// Decide OS
#include "GameSceneView.h"
#include "TextEditor.h"

void GameSceneView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void GameSceneView::on_gui()
{
	if (FileModule::Instance()->get_root() != NULL) {
	
        ImGui::SetNextWindowPos(ImVec2(window->get_width()*0.15625, 18));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.7, window->get_height()*0.7));
        ImGui::Begin("viewTabs", NULL, ImGuiWindowFlags_NoTitleBar);
        if (ImGui::BeginTabBar("MyTabBar"))
        {
            if (ImGui::BeginTabItem("Scene"))
            {
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
                unsigned int texture = RendererTemp::Instance()->get_rt();
                ImGui::Image((ImTextureID)texture, ImVec2(1120, 630), uv_min, uv_max, tint_col, border_col);
                //So, if you have the tex_id, just put it in the first argument and see what happens.
                //If you have the sizes, that's great, but if you don't, the size of the window is 1120 by 630, so try that.
                //If that doesn't work, then I'll be a bit sad.
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Game"))
            {
                ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Text Edit"))
            {
                static TextEditor editor;

                editor.SetShowWhitespaces(false);
                editor.SetReadOnly(false);
                editor.SetPalette(TextEditor::GetDarkPalette());
                editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
                editor.Render("##EditorWindow", ImVec2(1120, 630));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
	
	}
}

void GameSceneView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
