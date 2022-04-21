#include "MenuBarView.h"
#include <iostream>
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


	
        ImGui::SetNextWindowPos(ImVec2(window->get_width()*0.15625, 48));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.7+18, window->get_height()*0.7+11));
        ImGui::Begin("Script Editor", NULL, ImGuiWindowFlags_NoTitleBar);

        static TextEditor editor;

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
