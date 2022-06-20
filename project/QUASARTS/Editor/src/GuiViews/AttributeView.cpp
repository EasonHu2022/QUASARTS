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
				if (componentType == COMPONENT_TRANSFORM)
				{
					show_transform();
				}
				else if (componentType == COMPONENT_CAMERA)
				{
					ImGui::Text("Camera");
					Engine::CameraComponent* componentCa = Engine::ECSManager::Instance()->get_component<Engine::CameraComponent>(current_entity_id, componentType);
					ImGui::DragFloat("fov", &componentCa->fov);
					ImGui::DragFloat("ratio", &componentCa->ratio);
					ImGui::DragFloat("near", &componentCa->nearClip);
					ImGui::DragFloat("far", &componentCa->farClip);
				}
				else if (componentType == COMPONENT_LIGHTING)
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
				}
				else if (componentType == COMPONENT_MATERIAL)	
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
					ImGui::ColorEdit3("emission", glm::value_ptr(componentM->material->emission));
					ImGui::DragFloat("Blur Range", &componentM->material->emissiveRange);
					ImGui::DragFloat("shininess", &componentM->material->shininess);

					auto path = FileModule::Instance()->get_internal_assets_path();
					std::size_t pathLength = path.size();
					std::string temp = (componentM->material->texturePath).substr(pathLength);
					ImGui::Text(("Texture Path:   " + temp).c_str());
					if (ImGui::Button("Load Texture"))
					{
						#ifdef QS_WINDOWS
							std::string file_name = MenuBarView::OpenFileDialogue(L"All Files (*.*)\0*.png\0");
						#else
							std::string file_name = MenuBarView::OpenFileDialogue("\"\"*.png\" \"*.jpg\"\"");
						#endif

						if(file_name.compare("N/A") == 0) { continue; }

						std::string texturePath = file_name;
						std::string vShaderPath = componentM->material->vShaderPath;
						std::string fShaderPath = componentM->material->fShaderPath;
						std::string gShaderPath = componentM->material->gShaderPath;
						
						delete componentM->material;
						componentM->material = new Engine::Material(vShaderPath, fShaderPath, gShaderPath, texturePath);

						QDEBUG("Loading texture: {0}", texturePath);
					}
				}
				else if (componentType == COMPONENT_MESH)			
				{
					ImGui::Text("Mesh");
					Engine::MeshComponent* componentMe = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(current_entity_id, componentType);

					auto path = FileModule::Instance()->get_internal_assets_path();
					std::size_t pathLength = path.size();
					std::string temp = (componentMe->path).substr(pathLength);
					ImGui::Text(("Mesh Path:   " + temp).c_str());
					if (ImGui::Button("Load Mesh"))
					{
						#ifdef QS_WINDOWS
							std::string file_name = MenuBarView::OpenFileDialogue(L"All Files (*.*)\0*.obj\0");
						#else
							std::string file_name = MenuBarView::OpenFileDialogue("\"\"*.obj\"\"");
						#endif

						if(file_name.compare("N/A") == 0) { continue; }

						componentMe->path = file_name;

						QDEBUG("Loading mesh: {0}", componentMe->path);
					}
				}
				else if (componentType == COMPONENT_SCRIPT)				
				{
					ImGui::Text("Script");
					Engine::ScriptComponent* componentS = Engine::ECSManager::Instance()->get_component<Engine::ScriptComponent>(current_entity_id, componentType);
					ImGui::InputText("Script", componentS->script_path.data(), componentS->script_path.length()+1);
				}
				else if (componentType == COMPONENT_PARTICLE)
					show_particle();
				else if (componentType == COMPONENT_COLLISION_SPHERE)
				{
					ImGui::Text("Collision Sphere");
					Engine::CollisionSystem* collisionSys = Engine::CollisionSystem::Instance();
					Engine::CollisionSphereComponent* componentSp = Engine::ECSManager::Instance()->get_component<Engine::CollisionSphereComponent>(current_entity_id, componentType);
					if (ImGui::DragFloat3("offset", glm::value_ptr(componentSp->mLocalOffset), 0.1f, -10000.0f, 10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll))
						collisionSys->move_collision_component(current_entity_id, COMPONENT_COLLISION_SPHERE, componentSp->mLocalOffset);
					if (ImGui::DragFloat("collision radius", &componentSp->mRadius))
						collisionSys->set_collision_sphere_radius(current_entity_id, componentSp->mRadius);
				}
				else if (componentType == COMPONENT_ORBIT)
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
				}
				else if (componentType == COMPONENT_HEALTH)
				{
					ImGui::Text("Health");
					//Engine::HealthComponent* componentH = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(current_entity_id, componentType);
				}
				else if (componentType == COMPONENT_WEAPON)
				{
					ImGui::Text("Weapon");
				}
				else if (componentType == COMPONENT_ENEMY) {}
				else if (componentType == COMPONENT_ENEMY_SPAWNER)
				{
					ImGui::Text("Enemy Spawner");
					Engine::EnemySpawnComponent* componentEnSp = Engine::ECSManager::Instance()->get_component
											<Engine::EnemySpawnComponent>(current_entity_id, componentType);
					ImGui::DragFloat("Spawn Rate", &componentEnSp->spawnRate);
					ImGui::DragInt("Max Spawns", &componentEnSp->maxSpawns);

					int isActive = 0;
					int oneShot = 0;

					if (componentEnSp->isActive) { isActive = 1; }
					if (componentEnSp->oneShot) { oneShot = 1; }

					ImGui::Text("Activate");
					
					ImGui::RadioButton("On", &isActive, 1);
					ImGui::SameLine();
					ImGui::RadioButton("Off", &isActive, 0);

					if (isActive == 1) { componentEnSp->isActive = true; }
					else { componentEnSp->isActive = false; }

					ImGui::Text("One Shot");
					
					ImGui::RadioButton("True", &oneShot, 1);
					ImGui::SameLine();
					ImGui::RadioButton("False", &oneShot, 0);

					if (oneShot == 1) { componentEnSp->oneShot = true; }
					else { componentEnSp->oneShot = false; }
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
	
	int mode;
	if (particle->cone)
		mode = 1;
	else
		mode = 2;
	
	float dir[3] = { particle->direction.x, particle->direction.y, particle->direction.z };
	int rows = particle->texture.rows;

	if (ImGui::Button("Toggle On/Off")) {
		particle->is_on = !particle->is_on;
	}
	if (ImGui::Button("Toggle Particle Rotation")) {
		particle->randomRotation = !particle->randomRotation;
	}
	ImGui::RadioButton("Cone", &mode, 1); ImGui::SameLine();
	ImGui::RadioButton("Sphere", &mode, 2);
	if (mode == 1)
		particle->cone = true;
	else
		particle->cone = false;
	ImGui::InputFloat(" Particles per Second", &particle->pps);
	ImGui::InputFloat(" Gravity", &particle->gravity);
	ImGui::InputFloat(" Speed", &particle->averageSpeed);
	ImGui::InputFloat(" Lifetime", &particle->averageLifeLength);
	ImGui::InputFloat(" Scale", &particle->averageScale);
	ImGui::InputFloat(" Speed Error", &particle->speedError);
	ImGui::InputFloat(" Lifetime Error", &particle->lifeError);
	ImGui::InputFloat(" Scale Error", &particle->scaleError);
	ImGui::InputFloat(" Position Error", &particle->posError);
	ImGui::InputFloat3(" Direction", dir);
	ImGui::InputFloat(" Direction Deviation", &particle->directionDeviation);
	ImGui::InputInt(" Rows", &rows);
	if (rows < 1) { rows = 1; }
	
	particle->direction.x = dir[0];
	particle->direction.y = dir[1];
	particle->direction.z = dir[2];
	particle->texture.rows = rows;
	
	auto path = FileModule::Instance()->get_internal_assets_path();
	std::size_t pathLength = path.size();
	std::string temp;
	if (particle->path.empty())
	{
		temp = "";
	}
	else
	{
		temp = (particle->path).substr(pathLength);
	}
	ImGui::Text(("Texture Path:   " + temp).c_str());
	ImGui::SameLine();
	if (ImGui::Button("Load Atlas"))
	{
		#ifdef QS_WINDOWS
			std::string file_name = MenuBarView::OpenFileDialogue(L"All Files (*.*)\0*.png\0");
		#else
			std::string file_name = MenuBarView::OpenFileDialogue("\"\"*.png\" \"*.jpg\"\"");
		#endif

		if(file_name.compare("N/A") == 0) { return; }

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

		QDEBUG("Loading texture: {0}", file_name);
		particle2->loadtex(file_name.c_str());
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

void AttributeView::show_transform()
{
	// Text boxes:
	ImGui::Text("Transform");
	Engine::TransformComponent* componentTR = Engine::ECSManager::Instance()->get_component
								<Engine::TransformComponent>(current_entity_id, COMPONENT_TRANSFORM);
	ImGui::DragFloat3("position", glm::value_ptr(componentTR->position), 0.1f, -10000.0f,
				10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
	ImGui::DragFloat3("rotation", glm::value_ptr(componentTR->rotation), 0.1f, -10000.0f,
				10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);
	ImGui::DragFloat3("scale", glm::value_ptr(componentTR->scale), 0.1f, -10000.0f,
				10000.0f, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_AutoSelectAll);

	// Gizmos:
	static int operation = 0;

	ImGui::RadioButton("Translate", &operation, ImGuizmo::OPERATION::TRANSLATE);

	ImGui::SameLine();
	ImGui::RadioButton("Rotate", &operation, ImGuizmo::OPERATION::ROTATE);

	ImGui::SameLine();
	ImGui::RadioButton("Scale", &operation, ImGuizmo::OPERATION::SCALE);

	componentTR->operation = (ImGuizmo::OPERATION)(operation);
}