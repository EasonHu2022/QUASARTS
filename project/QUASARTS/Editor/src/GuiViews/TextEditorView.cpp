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
    /*if (FileModule::Instance()->get_root() != NULL) {
	
        ImGui::SetNextWindowPos(ImVec2(window->get_width()*0.15625, 48));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.5+18, window->get_height()*0.5+11));
        ImGui::Begin("Script Editor", NULL);

        static TextEditor editor;

        std::ifstream t("C://Users//alexc//Documents//Graphics//Project//EngineGenerates//Test_25_04_17//Assets//Resources//cube.obj");
        if (t.good())
        {
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            editor.SetText(str);
        }

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
