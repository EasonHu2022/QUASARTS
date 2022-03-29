#pragma once
#include "GuiViews/FileInputView.h"

void FileInputView::on_add()
{
    QDEBUG("on add view : FileInput");
}

void FileInputView::on_gui()
{
    if (FileModule::Instance()->get_root() != NULL) {
        
        
        ImGui::SetNextWindowPos(ImVec2(0, window->get_height()*0.7188888889));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.85625, window->get_height()*0.393333333));
        
        std::string assetsPath = FileModule::Instance()->get_root()->path;

#if defined(_WIN32)
        assetsPath = assetsPath + "\\";
#else
        assetsPath = assetsPath + "/";
#endif

        ImGui::Begin("Asset Selection", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        if (ImGui::TreeNode("Assets"))
        {

            std::string items[]{ "Resources","Scripts" };
            static const char* leaves[]{ "cool_object.obj","sphere.obj","another_object.obj" };
            std::string test_string = "test";
            static bool is_selected = true;
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;


            static int selection_mask = (1 << 2);
            int node_clicked = -1;
            for (int i = 0; i < 2; i++)
            {

                std::string tempAssetsPath = assetsPath + items[i];
                std::vector<std::filesystem::directory_entry> assetsFiles;

                for (auto const& dir_entry : std::filesystem::directory_iterator{ tempAssetsPath })
                {
                    assetsFiles.push_back(dir_entry);
                }

                ImGuiTreeNodeFlags node_flags = base_flags;

                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                // Items 0..2 are Tree Node
                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, items[i].c_str());
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = i;
                if (node_open) {
                    static int selected = -1;
                    for (int i = 0; i < assetsFiles.size(); i++)
                    {
                        if (ImGui::Selectable(assetsFiles[i].path().filename().string().c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick)) {

                            if (ImGui::IsMouseDoubleClicked(0)) {
                                if (FileModule::Instance()->get_root() == NULL)
                                    std::cout << "null" << std::endl;
                                if (FileModule::Instance()->get_root() != NULL)
                                    std::cout << FileModule::Instance()->get_root()->path << std::endl;
                            }
                                
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

        /*if (ImGui::TreeNode("ProjectSetting"))
        {
            std::string projectSettingPath = "D:\\Q2\\QUASARTS\\testing_input\\ProjectSetting";
            std::vector<std::filesystem::directory_entry> projectSettingFiles;
            for (auto const& dir_entry : std::filesystem::directory_iterator{ projectSettingPath })
            {
                projectSettingFiles.push_back(dir_entry);
            }
            static int selected = -1;
            for (int i = 0; i < projectSettingFiles.size(); i++)
            {
                if (ImGui::Selectable(projectSettingFiles[i].path().filename().string().c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick)) {

                    if (ImGui::IsMouseDoubleClicked(0))
                        std::cout << projectSettingFiles[i].path().string() << '\n';
                }
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Release"))
        {
            std::string releasePath = "D:\\Q2\\QUASARTS\\testing_input\\Release";
            std::vector<std::filesystem::directory_entry> releaseFiles;
            for (auto const& dir_entry : std::filesystem::directory_iterator{ releasePath })
            {
                releaseFiles.push_back(dir_entry);
            }
            static int selected = -1;
            for (int i = 0; i < releaseFiles.size(); i++)
            {
                if (ImGui::Selectable(releaseFiles[i].path().filename().string().c_str(), selected == i, ImGuiSelectableFlags_AllowDoubleClick)) {

                    if (ImGui::IsMouseDoubleClicked(0))
                        std::cout << releaseFiles[i].path().string() << '\n';
                }
            }

            ImGui::TreePop();
        }*/

        ImGui::End();
    
    }
}

void FileInputView::on_remove()
{
    QDEBUG("on remove view : FileInput");
}

