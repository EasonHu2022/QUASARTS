#pragma once
#include "GuiViews/FileInputView.h"
#include "QuasartsEngine.h"

void FileInputView::on_add()
{
    bool text_edit = false;
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
                                else if (i == 1) 
                                {
                                    if (size_t idk; Engine::ResourceManager::Instance()->load_resource(assetsFiles[i].path().string().c_str(), &idk))
                                    {
                                        auto luaFile = Engine::ResourceManager::Instance()->get_resource<Engine::FileResource>(idk);
                                        QDEBUG("Load File - {0}", luaFile->path);
                                        text_edit = true;
                                        //static cast 
                                        //QEditor* editor = (QEditor*)QEditor::Instance;
                                        //editor->getGuiView<TextEditorView>()->text_edit = true;
                                        //getGuiView<TextEditorView>()->text_edit = true;
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
    //QDEBUG("hi - {0}", new_project);
    if (text_edit == true) {
        ImGui::SetWindowFocus("Choose new project directory");

        ImGui::SetNextWindowSize(ImVec2(300, 100));
        ImGui::Begin("Script Editor", NULL, ImGuiWindowFlags_NoCollapse);
        static char buf1[64] = "";
        static char buf2[260] = "";
        //for (int i = 0; i < folder_path.length(); i++) {
        //    buf2[i] = folder_path[i];
        //}

        ImGui::PushItemWidth(-1);
        if (ImGui::InputTextWithHint("##pname", "Project Name", buf1, 64))
        {
            //project_name = buf1;
        }
        ImGui::PopItemWidth();
        if (ImGui::InputTextWithHint("##ppath", "Project Directory", buf2, 64)) {
            //folder_path = buf2;
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
        if (ImGui::Button("  Browse  ")) {
            //std::string temp_path = OpenFolderDialogue();
            //if (temp_path.compare("N/A") != 0)
            //    folder_path = temp_path;

        }
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
        if (ImGui::Button("Confirm")) {
            //if (strlen(buf1) != 0 && strlen(buf2) != 0) {

            //    char* cur_work_dir = getcwd(nullptr, 0);
            //    FileModule::Instance()->create_workdir(buf2, buf1);
            //    FileModule::Instance()->save_root(buf2, buf1, cur_work_dir);
            //    new_project = false;
            //    show_window = true;
            //    free(cur_work_dir);
            //}

        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 59);
        if (ImGui::Button("Cancel")) {
            text_edit = false;
        }
        ImGui::End();
    }


    /*if (FileModule::Instance()->get_root() != NULL) {

        ImGui::SetNextWindowPos(ImVec2(window->get_width()*0.15625, 48));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.5+18, window->get_height()*0.5+11));
        ImGui::Begin("Script Editor", NULL, ImGuiWindowFlags_NoTitleBar);

        static TextEditor editor;

        //std::ifstream t(fileToEdit);
        //if (t.good())
        //{
        //    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        //    editor.SetText(str);
        //}

        editor.SetShowWhitespaces(false);
        editor.SetReadOnly(false);
        editor.SetPalette(TextEditor::GetDarkPalette());
        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
        editor.Render("##EditorWindow", ImVec2(1120, 630));

        ImGui::End();

    }*/
}

void FileInputView::on_remove()
{
    QDEBUG("on remove view : FileInput");
}

