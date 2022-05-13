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
					ImGui::InputText("Material Path", componentM->path.data(), componentM->path.length()+1);
					ImGui::Text(componentM->material->name.data());
					ImGui::InputText("Vertex Shader", componentM->material->vShaderPath.data(), componentM->material->vShaderPath.length()+1);
					ImGui::InputText("Fragment Shader", componentM->material->fShaderPath.data(), componentM->material->fShaderPath.length() + 1);
					ImGui::InputText("Geometry Shader", componentM->material->gShaderPath.data(), componentM->material->gShaderPath.length() + 1);
					ImGui::ColorEdit3("ambient", glm::value_ptr(componentM->material->ambient));
					ImGui::ColorEdit3("diffuse", glm::value_ptr(componentM->material->diffuse));
					ImGui::ColorEdit3("specular", glm::value_ptr(componentM->material->specular));
					ImGui::DragFloat("shininess", &componentM->material->shininess);
					ImGui::InputText("texture", componentM->material->texturePath.data(), componentM->material->texturePath.length()+1);
					break;
				}					
				case COMPONENT_MESH:
				{
					ImGui::Text("Mesh");
					Engine::MeshComponent* componentMe = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(current_entity_id, componentType);
					ImGui::InputText("Mesh", componentMe->path.data(), componentMe->path.length() + 1);
					break;
				}					
				case COMPONENT_SCRIPT:
				{
					ImGui::Text("Script");
					Engine::ScriptComponent* componentS = Engine::ECSManager::Instance()->get_component<Engine::ScriptComponent>(current_entity_id, componentType);
					ImGui::InputText("Script", componentS->script_path.data(), componentS->script_path.length()+1);
					break;
				}
				case COMPONENT_PARTICLE:
					show_particle();
					break;
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


void AttributeView::show_particle() {
	
	ImGui::Separator();
	Engine::ParticleComponent* particle = Engine::ECSManager::Instance()->get_component<Engine::ParticleComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_PARTICLE);
	
	static bool is_on = particle->is_on;
	static bool randomRotation = particle->randomRotation;
	static int mode = 1;
	static float pps = particle->pps;
	static float gravity = particle->gravity;
	static float averageSpeed = particle->averageSpeed, averageLifeLength = particle->averageLifeLength, averageScale = particle->averageScale;
	static float speedError = particle->speedError, lifeError = particle->lifeError, scaleError = particle->scaleError;
	static float posError = particle->posError;
	static float directionDeviation = particle->directionDeviation;
	static float dir[3] = { particle->direction.x, particle->direction.y, particle->direction.z };

	if (ImGui::Button("Toggle On/Off")) {
		is_on = !is_on;
	}
	if (ImGui::Button("Toggle Particle Rotation")) {
		randomRotation = !randomRotation;
	}
	ImGui::RadioButton("Cone", &mode, 1); ImGui::SameLine();
	ImGui::RadioButton("Sphere", &mode, 2);
	if (mode == 1)
		particle->cone = true;
	else
		particle->cone = false;
	ImGui::InputFloat(" Particles per Second", &pps);
	ImGui::InputFloat(" Gravity", &gravity);
	ImGui::InputFloat(" Speed", &averageSpeed);
	ImGui::InputFloat(" Lifetime", &averageLifeLength);
	ImGui::InputFloat(" Scale", &averageScale);
	ImGui::InputFloat(" Speed Error", &speedError);
	ImGui::InputFloat(" Lifetime Error", &lifeError);
	ImGui::InputFloat(" Scale Error", &scaleError);
	ImGui::InputFloat(" Position Error", &posError);
	ImGui::InputFloat3(" Direction", dir);
	ImGui::InputFloat(" Direction Deviation", &directionDeviation);
	
	particle->is_on = is_on;
	particle->randomRotation = randomRotation;
	particle->pps = pps;
	particle->gravity = gravity;
	particle->averageSpeed = averageSpeed;
	particle->averageLifeLength = averageLifeLength;
	particle->averageScale = averageScale;
	particle->speedError = speedError;
	particle->lifeError = lifeError;
	particle->scaleError = scaleError;
	particle->posError = posError;
	particle->direction.x = dir[0];
	particle->direction.y = dir[1];
	particle->direction.z = dir[2];
	particle->directionDeviation = directionDeviation;
	
}

void AttributeView::change_transform(Engine::TransformComponent* transform, float* pos, float* rot, float* scal) {

}


void AttributeView::show_orbit()
{
	ImGui::Separator();
	Engine::OrbitComponent* orbit = Engine::ECSManager::Instance()->get_component<Engine::OrbitComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_ORBIT);
	static int primaryEntity = orbit->mPrimaryEntityId;
	static float orbitPeriod = orbit->mOrbitPeriod;
	static float normal[3] = { orbit->mAxisNormal.x, orbit->mAxisNormal.y, orbit->mAxisNormal.z };

	ImGui::InputInt(" Primary", &primaryEntity);
	ImGui::InputFloat(" Period", &orbitPeriod);
	ImGui::InputFloat3(" Normal", normal);

	orbit->mPrimaryEntityId = primaryEntity;
	orbit->mOrbitPeriod = orbitPeriod;
	orbit->mAxisNormal.x = normal[0];
	orbit->mAxisNormal.y = normal[1];
	orbit->mAxisNormal.z = normal[2];

} // change_transform()