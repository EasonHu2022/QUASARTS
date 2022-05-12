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
		ImGui::Begin("Attribute Window", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		
		if (!game) {
		
			if (Engine::ECSManager::Instance()->get_current_entity() != TOO_MANY_ENTITIES) {
				show_transform();
				if(Engine::ECSManager::Instance()->has_component(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_PARTICLE))
					show_particle();
			}
		
		}
		if (ImGui::Button("Game")) {
			game = !game;
		}
		if (ImGui::Button("Print")) {
			std::cout << Engine::Application::Instance->miniecs->entities[Engine::Application::Instance->miniecs->index]->attributes[0].x << std::endl;
		}
		ImGui::End();

	}
}

void AttributeView::on_remove()
{
	QDEBUG("on remove view : MenuBar");
}

void AttributeView::show_mesh() {
	Engine::MeshComponent* mesh = Engine::ECSManager::Instance()->get_component<Engine::MeshComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_MESH);
	std::string obj_name = "Name of mesh file: " + mesh->path.substr(mesh->path.find_last_of("\\") + 1);
	ImGui::Separator();
	ImGui::Text(obj_name.c_str());
}

void AttributeView::show_transform() {
	ImGui::Separator();
	Engine::TransformComponent* transform = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(Engine::ECSManager::Instance()->get_current_entity(), COMPONENT_TRANSFORM);
	static float pos[3] = { transform->position.x ,transform->position.y, transform->position.z };
	static float rot[3] = { transform->rotation.x ,transform->rotation.y, transform->rotation.z };
	static float scal[3] = { transform->scale.x ,transform->scale.y, transform->scale.z };

	static int mode = 0;
	ImGui::RadioButton("Translate", &mode, ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
	ImGui::RadioButton("Rotate", &mode, ImGuizmo::OPERATION::ROTATE); ImGui::SameLine();
	ImGui::RadioButton("Scale", &mode, ImGuizmo::OPERATION::SCALE);
	transform->operation = (ImGuizmo::OPERATION)mode;

	ImGui::InputFloat3(" Position", pos);
	ImGui::InputFloat3(" Rotation", rot);
	ImGui::InputFloat3(" Scale", scal);

	if (ImGui::IsWindowFocused()) {
		transform->position.x = pos[0];
		transform->position.y = pos[1];
		transform->position.z = pos[2];

		transform->rotation.x = rot[0];
		transform->rotation.y = rot[1];
		transform->rotation.z = rot[2];

		transform->scale.x = scal[0];
		transform->scale.y = scal[1];
		transform->scale.z = scal[2];
	}
	else {
		pos[0] = transform->position.x;
		pos[1] = transform->position.y;
		pos[2] = transform->position.z;
		rot[0] = transform->rotation.x;
		rot[1] = transform->rotation.y;
		rot[2] = transform->rotation.z;
		scal[0] = transform->scale.x;
		scal[1] = transform->scale.y;
		scal[2] = transform->scale.z;
	}
	
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
	particle->direction.x = dir[0];
	particle->direction.y = dir[1];
	particle->direction.z = dir[2];
	particle->directionDeviation = directionDeviation;
	
}