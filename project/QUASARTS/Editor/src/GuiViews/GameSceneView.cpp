#include "MenuBarView.h"
#include <iostream>
// Decide OS
#include "GameSceneView.h"
#include "TextEditor.h"

void GameSceneView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void GameSceneView::on_gui()
{
	if (FileModule::Instance()->get_root() != NULL) {


	
        ImGui::SetNextWindowPos(ImVec2(window->get_width()*0.15625, 18));
        ImGui::SetNextWindowSize(ImVec2(window->get_width()*0.7+18, window->get_height()*0.7+41));
        ImGui::Begin("viewTabs", NULL, ImGuiWindowFlags_NoTitleBar);

        if (ImGui::BeginTabBar("MyTabBar"))
        {
            if (ImGui::BeginTabItem("Scene"))
            {
                get_drag();
                ImVec2 uv_min = ImVec2(0.0f, 1.0f);                 // bottom-left
                ImVec2 uv_max = ImVec2(1.0f, 0.0f);                 // top-right
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
                unsigned int texture = Engine::Application::Instance->renderContext->get_renderTexture();
                ImGui::Image((ImTextureID)texture, ImVec2(1120, 630), uv_min, uv_max, tint_col, border_col);
                
                if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES) {

                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();
                    float windowWidth = (float)ImGui::GetWindowWidth();
                    float windowHeight = (float)ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                    auto projection = Engine::Application::Instance->renderContext->cameraContext->get_projection_data();
                    auto view = Engine::Application::Instance->renderContext->cameraContext->get_view_data();

                   // glm::mat4 actual = glm::inverse(glm::make_mat4(view));

                    Engine::TransformComponent* transform = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM);
                    auto obj_transform = Engine::get_transform_data(transform->position, transform->rotation, transform->scale);
                    //printf("%f\n", obj_transform[0]);
                    ImGuizmo::Manipulate(view, projection, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, obj_transform);
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Game"))
            {
                ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Text Edit"))
            {
                static TextEditor editor;

                editor.SetShowWhitespaces(false);
                editor.SetReadOnly(false);
                editor.SetPalette(TextEditor::GetDarkPalette());
                editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
                editor.Render("##EditorWindow", ImVec2(1120, 630));
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
	
	}
}

void GameSceneView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}

void GameSceneView::get_drag()
{
    if (io.MousePos.x > ImGui::GetWindowPos().x && io.MousePos.y > ImGui::GetWindowPos().y && io.MousePos.x < ImGui::GetWindowPos().x + ImGui::GetWindowWidth() && io.MousePos.y < ImGui::GetWindowPos().y + ImGui::GetWindowHeight()) {
        if (io.MouseDelta.x != 0 || io.MouseDelta.y != 0)
        {
            if (Engine::Input::get_mouse_combination({ Q_MOUSE_BUTTON_1 })) {
                std::cout << io.MouseDelta.x << "" << io.MouseDelta.y << std::endl;
            }
            else if (Engine::Input::get_mouse_combination({ Q_MOUSE_BUTTON_2 })) {
                std::cout << "left" << std::endl;
            }
        }
    }
}