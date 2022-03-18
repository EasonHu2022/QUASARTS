#include "MenuBarView.h"
#include <iostream>
// Decide OS
#if defined(_WIN32)
#include <windows.h>
#include <commdlg.h>
#endif
void MenuBarView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void MenuBarView::on_gui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project")) {



            }
            if (ImGui::MenuItem("Open Project", "Ctrl+O")) {

                std::cout << OpenFileDialogue().c_str() << std::endl;

            }
            if (ImGui::MenuItem("Save Project", "Ctrl+S")) {



            }
            ImGui::Separator();
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {



            }
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
            if (ImGui::MenuItem("Add Script")) {

            }
            if (ImGui::MenuItem("Delete Script")) {

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
}

void MenuBarView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}


#if defined(_WIN32)
std::string MenuBarView::OpenFileDialogue() {
    OPENFILENAME ofn;
    wchar_t fileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"All Files (*.*)\0*.cpp\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
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
#endif