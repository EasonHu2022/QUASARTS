#include "MenuBarView.h"
#include <iostream>
#include <cstring>
// Decide OS
#include "TextEditorView.h"
#include "TextEditor.h"

void TextEditorView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void TextEditorView::on_gui()
{
	if (FileModule::Instance()->get_root() != NULL) {

        //char fileToEdit[] = "C://Users/alexc/Documents/DocTextFile.txt";
	
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
	
	}
}

void TextEditorView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}
