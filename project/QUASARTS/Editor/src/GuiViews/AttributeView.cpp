#include "MenuBarView.h"
#include <iostream>
// Decide OS
#include "AttributeView.h"


void AttributeView::on_add()
{
	QDEBUG("on add view : MenuBar");
}

void AttributeView::on_gui()
{	
	if (FileModule::Instance()->get_root() != NULL) {

		ImGui::SetNextWindowPos(ImVec2(window->get_width() * 0.85625+18, 18));
		ImGui::SetNextWindowSize(ImVec2(window->get_width() * 0.34375, window->get_height() * 1.0922222222+41));
		ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		current_entity_id = Engine::ECSManager::Instance()->get_current_entity();
		if (current_entity_id == TOO_MANY_ENTITIES)
		{
			ImGui::End();
			return;
		};
		if (!game) {
			auto entityName = Engine::ECSManager::Instance()->get_entityName(current_entity_id);
			ImGui::Text(entityName.data());
			auto componentList = Engine::ECSManager::Instance()->get_all_component_types(current_entity_id);
			for (auto componentType : componentList)
			{
				switch (componentType)
				{
				case COMPONENT_TRANSFORM :
				{
					ImGui::Text("Transform");
					Engine::TransformComponent* componentTR = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(current_entity_id, componentType);
					ImGui::DragFloat3("position", glm::value_ptr(componentTR->position), 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
					ImGui::DragFloat3("rotation", glm::value_ptr(componentTR->rotation), 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
					ImGui::DragFloat3("scale", glm::value_ptr(componentTR->scale), 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
					break;
				}
				case COMPONENT_CAMERA:
				{
					ImGui::Text("Camera");
					Engine::CameraComponent* componentCa = Engine::ECSManager::Instance()->get_component<Engine::CameraComponent>(current_entity_id, componentType);
					ImGui::DragFloat("fov", &componentCa->fov);
					ImGui::DragFloat("ratio", &componentCa->ratio);
					ImGui::DragFloat("near", &componentCa->nearClip);
					ImGui::DragFloat("far", &componentCa->farClip);
					break;
				}
				case COMPONENT_LIGHTING:
				{
					ImGui::Text("Lighting");
					Engine::LightComponent* componentL = Engine::ECSManager::Instance()->get_component<Engine::LightComponent>(current_entity_id, componentType);
					const char* items[] = { "spot", "direction", "point" };
					int cur = (int)(componentL->type);
					ImGui::Combo("type", &cur, items, IM_ARRAYSIZE(items));
					componentL->type = (Engine::LightType)(cur);
					ImGui::ColorEdit3("ambient", glm::value_ptr(componentL->ambient));
					ImGui::ColorEdit3("diffuse", glm::value_ptr(componentL->diffuse));
					ImGui::ColorEdit3("specular", glm::value_ptr(componentL->specular));
				
					break;
				}		
				case COMPONENT_MATERIAL:
				{
					ImGui::Text("Material");
					Engine::MaterialComponent* componentM = Engine::ECSManager::Instance()->get_component<Engine::MaterialComponent>(current_entity_id, componentType);
					ImGui::InputText("Material Path", componentM->path.data(), IM_ARRAYSIZE(componentM->path.data()));
					ImGui::Text(componentM->material->name.data());
					ImGui::InputText("Vertex Shader", componentM->material->vShaderPath.data(), IM_ARRAYSIZE(componentM->material->vShaderPath.data()));
					ImGui::InputText("Fragment Shader", componentM->material->fShaderPath.data(), IM_ARRAYSIZE(componentM->material->fShaderPath.data()));
					ImGui::InputText("Geometry Shader", componentM->material->gShaderPath.data(), IM_ARRAYSIZE(componentM->material->gShaderPath.data()));
					ImGui::ColorEdit3("ambient", glm::value_ptr(componentM->material->ambient));
					ImGui::ColorEdit3("diffuse", glm::value_ptr(componentM->material->diffuse));
					ImGui::ColorEdit3("specular", glm::value_ptr(componentM->material->specular));
					ImGui::DragFloat("shininess", &componentM->material->shininess);
					ImGui::InputText("texture", componentM->material->texturePath.data(), IM_ARRAYSIZE(componentM->material->texturePath.data()));
					break;
				}					
				case COMPONENT_MESH:
				{
					ImGui::Text("Mesh");
					Engine::MeshComponent* componentMe = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(current_entity_id, componentType);
					ImGui::InputText("Mesh", componentMe->path.data(), IM_ARRAYSIZE(componentMe->path.data()));
					break;
				}					
				case COMPONENT_SCRIPT:
				{
					ImGui::Text("Script");
					Engine::ScriptComponent* componentS = Engine::ECSManager::Instance()->get_component<Engine::ScriptComponent>(current_entity_id, componentType);
					ImGui::InputText("Script", componentS->script_path.data(), IM_ARRAYSIZE(componentS->script_path.data()));
					break;
				}
				default:
					QERROR("unknown type : {0}", componentType);
					break;
				}				
			}	
		}
		ImGui::End();

	}
}

void AttributeView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}

