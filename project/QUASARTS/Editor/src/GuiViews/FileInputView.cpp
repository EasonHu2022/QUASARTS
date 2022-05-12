#pragma once
#include "GuiViews/FileInputView.h"
#include "TextEditor.h"
#include "QuasartsEngine.h"
#include <iostream>
#include <fstream>

void FileInputView::on_add()
{
    bool text_edit = false;
    TextEditor editor;
    std::string filePath = "";
    std::string fileText = "";
    QDEBUG("on add view : FileInput");
}

void FileInputView::on_gui()
{
    if (FileModule::Instance()->get_root() != NULL) {
        
        
        ImGui::SetNextWindowPos(ImVec2(0, window->get_height()*0.7188888889+41));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.85625+18, window->get_height()*0.393333333));
        
        std::string assetsPath = FileModule::Instance()->get_root()->path;

        assetsPath = assetsPath + "/";

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
                    for (int j = 0; j < assetsFiles.size(); j++)
                    {
                        if (ImGui::Selectable(assetsFiles[j].path().filename().string().c_str(), selected == j, ImGuiSelectableFlags_AllowDoubleClick)) {

                            if (ImGui::IsMouseDoubleClicked(0)) {
                                if (i == 0) {
                                    unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
                                    
                                    Engine::TransformComponent transform;
                                    transform.position = { 0.0f,0.0f, 0.0f };
                                    transform.rotation = { 0.0f,0.0f, 0.0f };
                                    transform.scale = { 1.0f,1.0f, 1.0f };
                                    Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM, transform);

                                    Engine::MeshComponent mesh;
                                    auto path = FileModule::Instance()->get_internal_assets_path();
                                    mesh.path = assetsFiles[j].path().string();
                                    Engine::ECSManager::Instance()->set_entityName(entityID, assetsFiles[j].path().filename().string());
                                    Engine::ECSManager::Instance()->create_component<Engine::MeshComponent>(entityID, COMPONENT_MESH, mesh);

                                    Engine::MaterialComponent material;
                                    //get default engine assets path
                                    std::string vshPath = path + "Shader/DefaultShader.vsh";
                                    std::string fshPath = path + "Shader/DefaultShader.fsh";
                                    std::string gshPth = "";
                                    std::string texturePath = path + "Texture/floor.jpg";
                                    material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
                                    Engine::ECSManager::Instance()->create_component<Engine::MaterialComponent>(entityID, COMPONENT_MATERIAL, material);
                                }
                                else if (i == 1) 
                                {
                                    if (size_t idk; Engine::ResourceManager::Instance()->load_resource(assetsFiles[j].path().string().c_str(), &idk))
                                    {
                                        auto luaFile = Engine::ResourceManager::Instance()->get_resource<Engine::FileResource>(idk);
                                        QDEBUG("Load File - {0}", luaFile->path);
                                        text_edit = true;
                                        filePath = luaFile->path;
                                        fileText = luaFile->textContent;
                                        QDEBUG("Current Text: {0}", fileText);
                                        editor.SetText(fileText);
                                    }
                                }
                            }
                                
                        }
                    }
                    ImGui::TreePop();
                }

            }
            ImGui::TreePop();
        }

        if (text_edit)
            show_text();

        ImGui::End();
    
    }
}

void FileInputView::show_text()
{
    if (text_edit == true) {

        ImGui::SetNextWindowSize(ImVec2(750, 750));
        ImGui::Begin(filePath.c_str(), 0);

        editor.SetShowWhitespaces(false);
        editor.SetReadOnly(false);
        editor.SetPalette(TextEditor::GetDarkPalette());
        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
        editor.Render("##EditorWindow", ImVec2(700, 650));

        ImGui::SameLine(ImGui::GetWindowWidth() - 59);
        if (ImGui::Button("Cancel")) {
            QDEBUG("File Closed: {0}", filePath.c_str());
            filePath = "";
            fileText = "";
            auto luaFile = NULL;
            text_edit = false;
            QDEBUG("Current Text: {0}", fileText);
        }
        if (ImGui::Button("Save")) {
            std::ofstream saveFile(filePath.c_str());
            saveFile << editor.GetText();
            saveFile.close();
            QDEBUG("File Saved: {0}", filePath.c_str());
            QDEBUG("Current Text: {0}", editor.GetText());
        }

        ImGui::End();
    }
}

void FileInputView::on_remove()
{
    QDEBUG("on remove view : FileInput");
}

