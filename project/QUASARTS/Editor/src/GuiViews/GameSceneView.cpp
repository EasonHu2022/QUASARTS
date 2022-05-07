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


                //imguizmo

                ImGuizmo::SetOrthographic(false);
                ImGuizmo::BeginFrame();
                ImGuizmo::SetDrawlist();
                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                auto projection = Engine::Application::Instance->renderContext->cameraContext->get_projection_data();
                auto view = Engine::Application::Instance->renderContext->cameraContext->get_view_data();

                if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES) {                              
                    Engine::TransformComponent* transform = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM);
                    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                    float matrix[] = {1.f, 0.f, 0.f, 0.f,
                                      0.f, 1.f, 0.f, 0.f,
                                      0.f, 0.f, 1.f, 0.f,
                                      0.f, 0.f, 0.f, 1.f 
                    };
                   
                    matrixTranslation[0] = transform->position.x;
                    matrixTranslation[1] = transform->position.y;
                    matrixTranslation[2] = transform->position.z;
                    matrixRotation[0] = transform->rotation.x;
                    matrixRotation[1] = transform->rotation.y;
                    matrixRotation[2] = transform->rotation.z;
                    matrixScale[0] = transform->scale.x;
                    matrixScale[1] = transform->scale.y;
                    matrixScale[2] = transform->scale.z;
                    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);
                    ImGuizmo::Manipulate(view, projection, (ImGuizmo::OPERATION)transform->operation, ImGuizmo::LOCAL, matrix);

                    ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

                    //update transform
                    
                    if (ImGuizmo::IsUsing()) {
                        if (transform->operation == ImGuizmo::OPERATION::TRANSLATE)
                            transform->position = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
                        else if(transform->operation == ImGuizmo::OPERATION::ROTATE)
                            transform->rotation = glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
                        else
                            transform->scale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);
                    }
                }
                //ImGuizmo::DrawGrid(view, projection, &glm::mat4(1.0)[0][0], 100.f);
                ImGui::EndTabItem();
             
            }
            if (ImGui::BeginTabItem("Game"))
            {
              
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