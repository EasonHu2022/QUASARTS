#pragma once
#include "GuiViews/FileInputView.h"

void FileInputView::on_add()
{
    QDEBUG("on add view : FileInput");
}

void FileInputView::on_gui()
{
    ImGui::Begin("New Window");

    if (ImGui::TreeNode("Assets"))
    {

        static const char* items[]{ "Objects","Textures","Animations" };
        static const char* leaves[]{ "cool_object.obj","sphere.obj","another_object.obj" };
        static bool is_selected = true;
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool align_label_with_current_x_position = false;
        static bool test_drag_and_drop = false;
        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

        static int selection_mask = (1 << 2);
        int node_clicked = -1;
        for (int i = 0; i < 3; i++)
        {

            ImGuiTreeNodeFlags node_flags = base_flags;

            const bool is_selected = (selection_mask & (1 << i)) != 0;
            if (is_selected)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            // Items 0..2 are Tree Node
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, items[i]);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = i;
            if (node_open) {
                static int selected = -1;
                for (int n = 0; n < 3; n++)
                {


                    if (ImGui::Selectable(leaves[n], selected == n, ImGuiSelectableFlags_AllowDoubleClick)) {
                        selected = n;
                        if (ImGui::IsMouseDoubleClicked(0))
                            QDEBUG("   --  {0}",leaves[n]);
                    }

                }
                ImGui::TreePop();
            }

        }
        if (node_clicked != -1)
        {
            // Update selection state
            // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
            if (ImGui::GetIO().KeyCtrl)
                selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
            else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
                selection_mask = (1 << node_clicked);           // Click to single-select
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

void FileInputView::on_remove()
{
    QDEBUG("on remove view : FileInput");
}

