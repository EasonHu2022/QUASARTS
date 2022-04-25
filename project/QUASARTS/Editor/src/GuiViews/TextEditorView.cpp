#include <iostream>
#include <cstring>
// Decide OS
#include "GuiViews/TextEditorView.h"
#include "GuiViews/TextEditor.h"

void TextEditorView::on_add()
{
    text_edit = false;
	QDEBUG("on add view : MenuBar");
    QDEBUG("on add view : TextEditorView");
}

void TextEditorView::on_gui()
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

void TextEditorView::edit_text(bool value)
{
    text_edit = value;
}

void TextEditorView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
