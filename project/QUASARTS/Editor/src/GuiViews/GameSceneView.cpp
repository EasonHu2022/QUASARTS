#include "MenuBarView.h"
#include <iostream>
// Decide OS
#include "GameSceneView.h"

void GameSceneView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void GameSceneView::on_gui()
{
    ImGui::SetNextWindowPos(ImVec2(250, 18));
    ImGui::SetNextWindowSize(ImVec2(1120, 630));
    ImGui::Begin("viewTabs", NULL, ImGuiWindowFlags_NoTitleBar);
    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Scene"))
        {
            ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah"); //Probably delete this when you put in the texture
            //ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            //ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            //ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            //ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            //ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
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
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void GameSceneView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
