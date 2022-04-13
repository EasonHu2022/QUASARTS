#include "MenuBarView.h"
#include <iostream>
// Decide OS
#if defined(_WIN32)
#include <windows.h>
#include <commdlg.h>
#include <ShlObj_core.h>
#endif
void MenuBarView::on_add()
{
    new_project = false;
    new_scene = false;
    folder_path = "";
	QDEBUG("on add view : MenuBar");
}

void MenuBarView::on_gui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Project", NULL, &new_project);
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {

                std::cout << OpenFileDialogue().c_str() << std::endl;

            }
            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {

                // scene test
                unsigned int entityID = Engine::ECSManager::Instance()->create_entity();
                std::string testing = "testing";
                Engine::Entity* ent = Engine::ECSManager::Instance()->get_entity(entityID);
                QDEBUG("MenuItem: Save Project");
                Engine::ECSManager::Instance()->print_entities();
                char msg[512];
                snprintf(msg, 512, "Entity name : %s", ent->get_name().c_str());
                QDEBUG(msg);
                // end scene test
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

                Engine::ScriptsSys::Instance()->deleteScript();
            }
            if (ImGui::MenuItem("Update Script(temp)")) {

                Engine::ScriptsSys::Instance()->updateScript();
            }
            if (ImGui::MenuItem("Add Attribute")) {

            }
            if (ImGui::MenuItem("Delete Attribute")) {

            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Object"))
        {
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
    ofn.lpstrFilter = L"All Files (*.*)\0*.cpp\0";
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

    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::Begin("Choose new porject directory", &new_project, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";
    static char buf2[260] = "";
    for (int i = 0; i < folder_path.length(); i++) {
        buf2[i] = folder_path[i];
    }

    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Project Name", buf1, 64);
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
            FileModule::Instance()->create_workdir(buf2, buf1);
            new_project = false;
            show_window = true;
        }

    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 59);
    if (ImGui::Button("Cancel")) {
        new_project = false;
    }

    ImGui::End();

}

void MenuBarView::newScene() {

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

    ImGui::SetNextWindowSize(ImVec2(250, 80));
    ImGui::Begin("Choose Script Name", &new_script, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    static char buf1[64] = "";


    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##pname", "Script Name", buf1, 64);
    ImGui::PopItemWidth();
    

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetWindowWidth() - 130);
    if (ImGui::Button("Confirm")) {
        if (strlen(buf1) != 0) {

            //create and add maybe?
            Engine::ScriptsSys::Instance()->createScript(buf1);
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