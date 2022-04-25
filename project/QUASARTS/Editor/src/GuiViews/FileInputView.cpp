#pragma once
#include "GuiViews/FileInputView.h"

void FileInputView::on_add()
{
    QDEBUG("on add view : FileInput");
}

void FileInputView::on_gui()
{
    if (FileModule::Instance()->get_root() != NULL) {
        
        
        ImGui::SetNextWindowPos(ImVec2(0, window->get_height()*0.7188888889+41));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.85625+18, window->get_height()*0.393333333));
        
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
                    if(i == 0 && dir_entry.path().extension().compare(".obj") == 0)
                        assetsFiles.push_back(dir_entry);
                    if (i == 1 && dir_entry.path().extension().compare(".lua") == 0)
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
                                if (i == 0) {
                                    unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
                                    Engine::ECSManager::Instance()->set_entityName(entityID, "object");
                                    Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM);
                                    Engine::TransformComponent transform;
                                    transform.position = { 0.0f,0.0f, 0.0f };
                                    Engine::ECSManager::Instance()->replace_component(entityID, COMPONENT_TRANSFORM, transform);


                                    Engine::ECSManager::Instance()->create_component<Engine::MeshComponent>(entityID, COMPONENT_MESH);
                                    Engine::MeshComponent mesh;
                                    mesh.path = assetsFiles[i].path().string();
                                    Engine::ECSManager::Instance()->replace_component(entityID, COMPONENT_MESH, mesh);

                                    Engine::ECSManager::Instance()->create_component<Engine::MaterialComponent>(entityID, COMPONENT_MATERIAL);
                                    Engine::MaterialComponent material;
                                    material.material = new Engine::Material("D:\\Q6\\QUASARTS\\project\\QUASARTS\\Engine\\src\\Shader\\DefaultShader.vsh", "D:\\Q6\\QUASARTS\\project\\QUASARTS\\Engine\\src\\Shader\\DefaultShader.fsh");
                                    Engine::ECSManager::Instance()->replace_component(entityID, COMPONENT_MATERIAL, material);
                                }
                                else {}
                            }
                                
                        }
                    }
                    ImGui::TreePop();
                }

            }
            ImGui::TreePop();
        }

        ImGui::End();
    
    }
}

void FileInputView::on_remove()
{
    QDEBUG("on remove view : FileInput");
}

