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
				case COMPONENT_COLLISION_SPHERE:
				{
					ImGui::Text("Collision Sphere");
					Engine::CollisionSystem* collisionSys = Engine::CollisionSystem::Instance();
					Engine::CollisionSphereComponent* componentSp = Engine::ECSManager::Instance()->get_component<Engine::CollisionSphereComponent>(current_entity_id, componentType);
					if (ImGui::DragFloat3("offset", glm::value_ptr(componentSp->mLocalOffset), 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll))
						collisionSys->move_collision_component(current_entity_id, COMPONENT_COLLISION_SPHERE, componentSp->mLocalOffset);
					if (ImGui::DragFloat("collision radius", &componentSp->mRadius))
						collisionSys->set_collision_sphere_radius(current_entity_id, componentSp->mRadius);
					break;
				}
				case COMPONENT_ORBIT:
				{
					ImGui::Text("Orbit");
					Engine::OrbitSystem* orbitSys = Engine::OrbitSystem::Instance();
					Engine::OrbitComponent* componentO = Engine::ECSManager::Instance()->get_component<Engine::OrbitComponent>(current_entity_id, componentType);

					if (ImGui::InputInt("primary", &componentO->mPrimaryEntityId, 0, 0))
					{
						if (-1 == orbitSys->set_orbit_primary(current_entity_id, componentO->mPrimaryEntityId))
							componentO->mActive = false;
					}
					ImGui::InputFloat("period", &componentO->mOrbitPeriod);
					ImGui::DragFloat3("normal", glm::value_ptr(componentO->mAxisNormal), 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);					
					if (ImGui::Button("Activate"))
					{
						if (componentO->mPrimaryEntityId != -1) {
							componentO->mActive = true;
							if (-1 == orbitSys->initialise_orbit(current_entity_id))
								componentO->mActive = false;
						}
					}
					if (ImGui::Button("Deactivate"))
					{
						componentO->mActive = false;
					}
					ImGui::Text("Orbit: Computed Parameters");
					ImGui::InputFloat("orbit radius", &componentO->mDistPeriapse, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
					ImGui::InputFloat3("X-axis", glm::value_ptr(componentO->mAxisX), "%.3f", ImGuiInputTextFlags_ReadOnly);
					ImGui::InputFloat3("Y-axis", glm::value_ptr(componentO->mAxisY), "%.3f", ImGuiInputTextFlags_ReadOnly);
					ImGui::InputFloat("true anomaly", &componentO->mTrueAnomDeg, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
					break;
				}
				case COMPONENT_HEALTH:
					ImGui::Text("Health");
					//Engine::HealthComponent* componentH = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(current_entity_id, componentType);
					break;
				case COMPONENT_WEAPON:
					ImGui::Text("Weapon");
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

	static int rows = particle->texture.rows;

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
	ImGui::InputInt(" Rows", &rows);
	
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
	particle->texture.rows = rows;
	
	static char buf1[260] = "";

	ImGui::InputText("Texture Path", &buf1[0], IM_ARRAYSIZE(buf1));

	ImGui::SameLine;
	if (ImGui::Button("Load"))
	{
		bool randomRotation = particle->randomRotation;
		bool mode = particle->cone;
		bool is_on = particle->is_on;
		float pps = particle->pps;
		float gravity = particle->gravity;
		float averageSpeed = particle->averageSpeed, averageLifeLength = particle->averageLifeLength, averageScale = particle->averageScale;
		float speedError = particle->speedError, lifeError = particle->lifeError, scaleError = particle->scaleError;
		float posError = particle->posError;
		float directionDeviation = particle->directionDeviation;
		float dir[3] = { particle->direction.x, particle->direction.y, particle->direction.z };
		int rows = particle->texture.rows;

		Engine::ECSManager::Instance()->destroy_component<Engine::ParticleComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_PARTICLE);
		Engine::ECSManager::Instance()->create_component<Engine::ParticleComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_PARTICLE);
		Engine::ParticleComponent* particle2 = Engine::ECSManager::Instance()->get_component<Engine::ParticleComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_PARTICLE);

		particle2->is_on = is_on;
		particle2->randomRotation = randomRotation;
		particle2->pps = pps;
		particle->gravity = gravity;
		particle2->averageSpeed = averageSpeed;
		particle2->averageLifeLength = averageLifeLength;
		particle2->averageScale = averageScale;
		particle2->speedError = speedError;
		particle2->lifeError = lifeError;
		particle2->scaleError = scaleError;
		particle2->posError = posError;
		particle2->direction.x = dir[0];
		particle2->direction.y = dir[1];
		particle2->direction.z = dir[2];
		particle2->directionDeviation = directionDeviation;
		particle2->texture.rows = rows;
		particle2->cone = mode;

		particle2->loadtex(buf1);
	}
	
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