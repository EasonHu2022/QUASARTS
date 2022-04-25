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
                                //std::cout << assetsFiles[i].path().string().c_str() << std::endl;
                               /* std::unordered_map<std::string, std::shared_ptr<Engine::Mesh>> meshes{};
                                Engine::Application::Instance -> loaderFactory->load(assetsFiles[i].path().string().c_str(), meshes);
                                auto model = new Engine::ModelResource();
                                auto ent = new Engine::AttributeVector();
                                ent->attributes[0].x = 0;
                                ent->attributes[0].y = 0;
                                ent->attributes[0].z = 0;
                                ent->attributes[1].x = 0;
                                ent->attributes[1].y = 0;
                                ent->attributes[1].z = 0;
                                model->meshes = meshes;
                                model->name = assetsFiles[i].path().filename().string().c_str();
                                Engine::Application::Instance->entityWorld->add_entity(model);
                                Engine::Application::Instance->miniecs->add_entity(ent);*/

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

