#include "MenuBarView.h"
#include <iostream>
// Decide OS
#if defined(_WIN32)
#include <windows.h>
#include <commdlg.h>
#include <direct.h>
#include <ShlObj_core.h>
#endif
void MenuBarView::on_add()
{
    new_project = false;
    new_scene = false;
    new_entity = false;
    new_child = false;
    new_script = false;
    folder_path = "";
    project_name = "";
    QDEBUG("on add view : MenuBar");
}

void MenuBarView::on_gui()
{
    //QDEBUG("hi - {0}", new_project);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Project", NULL, &new_project);
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {

                //std::cout << OpenFileDialogue().c_str() << std::endl;
                FileModule::Instance()->open_root(OpenFileDialogue());

            }
            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {

            }
            ImGui::Separator();
            ImGui::MenuItem("New Scene", "Ctrl+N", &new_scene);
            if (ImGui::MenuItem("Open Scene", "Ctrl+Shift+O")) {



            }
            if (ImGui::MenuItem("Save Scene", "Ctrl+Shift+S")) {



            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Select All", "Ctrl+A")) {

            }
            if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {

            }
            if (ImGui::MenuItem("Select Children", "Shift+C")) {

            }
            if (ImGui::MenuItem("Invert Selection", "Ctrl+I")) {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {

            }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {

            }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Play", "Ctrl+P")) {

                Engine::ScriptSystem::Instance()->reloadScript();
                Engine::ScriptSystem::Instance()->importFunc();
            }
            if (ImGui::MenuItem("Pause", "Ctrl+Shift+P")) {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("Add Path")) {

            }
            ImGui::MenuItem("Add Script", NULL, &new_script);
            if (ImGui::MenuItem("Delete Script")) {

                Engine::ScriptSystem::Instance()->deleteScript();
            }
            if (ImGui::MenuItem("Add Attribute")) {
                if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES)
                    new_attribute = true;
            }
            if (ImGui::MenuItem("Delete Attribute")) {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Object"))
        {
            ImGui::MenuItem("Blank Entity", NULL, &new_entity);
            if (ImGui::MenuItem("Child Entity")) {
                if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES)
                    new_child = true;
            }
            if (ImGui::BeginMenu("Insert Basic Object")) {
                if (ImGui::MenuItem("Triangle")) {
                    
                }
                if (ImGui::MenuItem("Plane")) {
                  
                }
                if (ImGui::MenuItem("Pyramid")) {

                }
                if (ImGui::MenuItem("Cube")) {

                }
                if (ImGui::MenuItem("Cone")) {

                }
                if (ImGui::MenuItem("Sphere")) {

                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Camera")) {

            }
            if (ImGui::MenuItem("Light")) {

                unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
                Engine::ECSManager::Instance()->set_entityName(entityID, "light");
                Engine::ECSManager::Instance()->create_component<Engine::LightComponent>(entityID, COMPONENT_LIGHTING);
                Engine::LightComponent light;
                light.ambient = { 1.0f,0.0f, 0.0f };
                light.type = Engine::LightType::point;
                Engine::ECSManager::Instance()->replace_component(entityID, COMPONENT_LIGHTING, light);

                Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM);
                Engine::TransformComponent transform;
                transform.position = { 1.0f,2.5f, 0.0f };
                Engine::ECSManager::Instance()->replace_component(entityID, COMPONENT_TRANSFORM, transform);
            }
            if (ImGui::MenuItem("Particle Emitter")) {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (new_project)
        newProject();
    if (new_scene)
        newScene();
    if (new_script)
        newScript();
    if (new_entity)
        newEntity();
    if (new_child)
        newChild();
    if (new_attribute)
        newAttribute();
}

void MenuBarView::on_remove()
{
    QDEBUG("on remove view : MenuBar");
}


#if defined(_WIN32)
std::string MenuBarView::OpenFileDialogue() {
    OPENFILENAME ofn;
    wchar_t fileName[260] = L"";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"All Files (*.*)\0*.q\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = 260;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"";

    if (GetOpenFileName(&ofn)) {
        std::wstring ws(fileName);
        // your new String
        std::string fileNameStr(ws.begin(), ws.end());
        return fileNameStr;
    }
    else
        return "N/A";
}
std::string MenuBarView::OpenFolderDialogue() {
    wchar_t path[260] = L"";
    BROWSEINFO bi;

    bi.hwndOwner = NULL;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = path;	// This is just for display: not useful
    bi.lpszTitle = L"Choose Client Directory";
    bi.ulFlags = BIF_RETURNONLYFSDIRS;
    bi.lpfn = NULL;
    bi.lParam = 0;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (SHGetPathFromIDList(pidl, path)) {
        std::wstring ws(path);
        std::string pathstr(ws.begin(), ws.end());
        //Engine::Application::Instance->loaderFactory->load(pathstr, pathstr);
        return pathstr;
    }
    else
        return "N/A";
}
#else
std::string MenuBarView::OpenFileDialogue() {
    char filename[1024];
    FILE* f = popen("zenity --file-selection --file-filter=*.cpp", "r");
    if (f == NULL)
        return "N/A";
    else {
        fgets(filename, 1024, f);
        std::string fileNameStr;
        fileNameStr = filename;
        return fileNameStr;
    }

}
std::string MenuBarView::OpenFolderDialogue() {
    char foldername[1024];
    FILE* f = popen("zenity  --file-selection --directory", "r");
    if (f == NULL)
        return "N/A";
    else {
        fgets(foldername, 1024, f);
        std::string folderNameStr;
        folderNameStr = foldername;
        return folderNameStr;
    }

}
#endif

void MenuBarView::newProject() {

    ImGui::SetWindowFocus("Choose new project directory");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose new project directory", &new_project, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";
    static char buf2[260] = "";
    for (int i = 0; i < folder_path.length(); i++) {
        buf2[i] = folder_path[i];
    }

    ImGui::PushItemWidth(-1);
    if (ImGui::InputTextWithHint("##pname", "Project Name", buf1, 64))
    {
        project_name = buf1;
    }
    ImGui::PopItemWidth();
    if (ImGui::InputTextWithHint("##ppath", "Project Directory", buf2, 64)) {
        folder_path = buf2;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
    if (ImGui::Button("  Browse  ")) {
        std::string temp_path = OpenFolderDialogue();
        if (temp_path.compare("N/A") != 0)
            folder_path = temp_path;

    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0 && strlen(buf2) != 0) {
            
            char* cur_work_dir = getcwd(nullptr, 0);
            FileModule::Instance()->create_workdir(buf2, buf1);
            FileModule::Instance()->save_root(buf2, buf1, cur_work_dir);
            new_project = false;
            show_window = true;
            free(cur_work_dir);
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_project = false;
    }

    ImGui::End();

}

void MenuBarView::newScene() {

    ImGui::SetWindowFocus("Choose Scene Name");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Scene Name", &new_scene, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";


    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Project Name", buf1, 64);
    ImGui::PopItemWidth();


    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            new_scene = false;
            show_window = true;
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_scene = false;
    }

    ImGui::End();

}

void MenuBarView::newScript() {

    ImGui::SetWindowFocus("Choose Script Name");

    ImGui::SetNextWindowSize(ImVec2(250, 80));
    ImGui::Begin("Choose Script Name", &new_script, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";


    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Script Name(plz use test for now)", buf1, 64);
    ImGui::PopItemWidth();


    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            if (!(folder_path.empty() || project_name.empty()))
            {
#if defined(_WIN32)
                std::string file_path = folder_path + "\\" + project_name;
#else
                std::string file_path = folder_path + "/" + project_name;
#endif
                Engine::ScriptSystem::Instance()->createScript(buf1, file_path);
            }
            else 
            {
                QWARN("Failed to create the script, Please create a project first");
            }
            new_script = false;
            show_window = true;
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_script = false;
    }

    ImGui::End();

}

void MenuBarView::newEntity() {

    ImGui::SetWindowFocus("Choose Entity Name");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Entity Name", &new_entity, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";


    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Entity Name", buf1, 64);
    ImGui::PopItemWidth();


    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
            Engine::ECSManager::Instance()->set_entityName(entityID, buf1);
            new_entity = false;
            buf1[0] = '\0';
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_entity = false;
    }

    ImGui::End();

}

void MenuBarView::newChild() {

    ImGui::SetWindowFocus("Choose Child Name");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Child Name", &new_child, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";


    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Entity Name", buf1, 64);
    ImGui::PopItemWidth();


    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            unsigned int entityID = Engine::ECSManager::Instance()->create_entity();

            Engine::ECSManager::Instance()->set_entityName(entityID, buf1);
            Engine::ECSManager::Instance()->add_child(Engine::ECSManager::Instance()->get_current_entity(), entityID);
            new_child = false;
            buf1[0] = '\0';

        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_child = false;
    }

    ImGui::End();

}

void MenuBarView::newAttribute() {

    ImGui::SetWindowFocus("Choose Attribute Type");

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose Attribute Type", &new_attribute, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    static int item_current_idx = 0; // Here we store our selection data as an index.
    const char* combo_preview_value = components[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
    if (ImGui::BeginCombo("Attributes", combo_preview_value))
    {
        for (int n = 0; n < NUM_COMPONENT_TYPES; n++)
        {
            if (ImGui::Selectable(components[n]))
                item_current_idx = n;
        }
        ImGui::EndCombo();
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if(item_current_idx == COMPONENT_TRANSFORM)
            Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(Engine::ECSManager::Instance()->get_current_entity(), item_current_idx);
        else if(item_current_idx == COMPONENT_MESH)
            Engine::ECSManager::Instance()->create_component<Engine::MeshComponent>(Engine::ECSManager::Instance()->get_current_entity(), item_current_idx);
        else if (item_current_idx == COMPONENT_COLLISION_SPHERE)
            Engine::ECSManager::Instance()->create_component<Engine::CollisionSphereComponent>(Engine::ECSManager::Instance()->get_current_entity(), item_current_idx);
        new_attribute = false;
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_attribute = false;
    }

    ImGui::End();

}