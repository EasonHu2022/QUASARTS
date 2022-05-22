#include "ComponentsExporter.h"

namespace Engine {

	void ComponentsExporter::exportComp(sol::state& p_lua_state)
	{
		//transform attributes
		p_lua_state.set_function("updatePosition", &updatePosition);
		p_lua_state.set_function("updateRotation", &updateRotation);
		p_lua_state.set_function("updateScale", &updateScale);
		p_lua_state.set_function("getPosition", &getPosition);

		//camera
		p_lua_state.set_function("getCamera", &getCamera);
		p_lua_state.set_function("cameraZoomIn", &camera_zoom_in);
		p_lua_state.set_function("cameraZoomOut", &camera_zoom_out);

		//health 
		p_lua_state.set_function("setMaxHealth", &setMaxHealth);
		p_lua_state.set_function("setCurrentHealth", &setCurrentHealth);
		p_lua_state.set_function("getCurrentHealth", &getCurrentHealth);

		//weapon
		p_lua_state.set_function("setDamage", &setDamage);
		p_lua_state.set_function("setRange", &setRange);

		//create new entity and add components
		p_lua_state.set_function("createEntity", &createEntity);
		p_lua_state.set_function("addComponent", &addComponent);

		// Collisions
		p_lua_state.set_function("getNumCollidedEntities", &getNumCollidedEntities);
		p_lua_state.set_function("getCollidedEntity", &getCollidedEntity);
	}


	unsigned int getCamera()
	{
		auto camera = ECSManager::Instance()->get_camera();
		return camera;
	}

	void camera_zoom_in(unsigned int id)
	{
		Engine::CameraComponent* camera = Engine::ECSManager::Instance()->get_component<Engine::CameraComponent>(id, COMPONENT_CAMERA);
		camera->fov -= 0.5;
	}

	void camera_zoom_out(unsigned int id)
	{
		Engine::CameraComponent* camera = Engine::ECSManager::Instance()->get_component<Engine::CameraComponent>(id, COMPONENT_CAMERA);
		camera->fov += 0.5;
	}

	//transform
	static void updatePosition(unsigned int id, glm::vec3 dT)
	{
		Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
		trs->position = trs->position + dT;
	}

	static void updateRotation(unsigned int id, glm::vec3 dR)
	{
		Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
		trs->rotation = trs->rotation + dR;
	}

	static void updateScale(unsigned int id, glm::vec3 dS)
	{
		Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
		trs->scale = trs->scale + dS;
	}

	glm::vec3 getPosition(unsigned int id)
	{
		Engine::TransformComponent* trs = Engine::ECSManager::Instance()->get_component<Engine::TransformComponent>(id, COMPONENT_TRANSFORM);
		return trs->position;
	}


	//health
	static void setMaxHealth(unsigned int id, float maxH)
	{
		Engine::HealthComponent* health = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(id, COMPONENT_HEALTH);
		health->max_health = maxH;
	}


	static void setCurrentHealth(unsigned int id, float curr_health)
	{
		Engine::HealthComponent* health = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(id, COMPONENT_HEALTH);
		health->current_health = curr_health;
	}

	static float getCurrentHealth(unsigned int id)
	{
		Engine::HealthComponent* health = Engine::ECSManager::Instance()->get_component<Engine::HealthComponent>(id, COMPONENT_HEALTH);
		return health->current_health;
	}


	//weapon 
	static void setDamage(unsigned int id, float damage)
	{
		Engine::WeaponComponent* weapon = Engine::ECSManager::Instance()->get_component<Engine::WeaponComponent>(id, COMPONENT_WEAPON);
		weapon->base_damage = damage;
	}

	static void setRange(unsigned int id, float range)
	{
		Engine::WeaponComponent* weapon = Engine::ECSManager::Instance()->get_component<Engine::WeaponComponent>(id, COMPONENT_WEAPON);
		weapon->attack_range = range;
	}

	static void setSpeed(unsigned int id, int speed)
	{
		Engine::WeaponComponent* weapon = Engine::ECSManager::Instance()->get_component<Engine::WeaponComponent>(id, COMPONENT_WEAPON);
		weapon->attack_speed = speed;
	}

	//create entities
	static unsigned int createEntity(const std::string& entity_name)
	{
		unsigned int entityID = Engine::ECSManager::Instance()->create_entity();

			Engine::ECSManager::Instance()->set_entityName(entityID, entity_name);
			Engine::TransformComponent transform;
			transform.position = { 0.0f,0.0f, 0.0f };
			transform.rotation = { 0.0f,0.0f, 0.0f };
			transform.scale = { 1.0f,1.0f, 1.0f };
			Engine::ECSManager::Instance()->create_component<Engine::TransformComponent>(entityID, COMPONENT_TRANSFORM, transform);

			Engine::MeshComponent mesh;
			auto path = FileModule::Instance()->get_internal_assets_path();
			if ("sphere" == entity_name)
			{
				mesh.path = path + "DefaultObjects/sphere20x20.obj";
			}

			if ("triangle" == entity_name)
			{
				mesh.path = path + "DefaultObjects/triangle.obj";
			}

			Engine::ECSManager::Instance()->create_component<Engine::MeshComponent>(entityID, COMPONENT_MESH, mesh);

			Engine::MaterialComponent material;
			//get default engine assets path
			std::string vshPath = path + "Shader/DefaultShader.vsh";
			std::string fshPath = path + "Shader/DefaultShader.fsh";
			std::string gshPth = "";
			std::string texturePath = path + "Texture/white.png";
			material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
			Engine::ECSManager::Instance()->create_component<Engine::MaterialComponent>(entityID, COMPONENT_MATERIAL, material);
		return entityID;
	}

	//add components to entity
	static void addComponent(unsigned int entity_id, const std::string& component_type)
	{

		if (component_type == "Health")
		{
			Engine::ECSManager::Instance()->create_component<Engine::HealthComponent>(entity_id, COMPONENT_HEALTH);
		}

		else if (component_type == "Weapon")
		{
			Engine::ECSManager::Instance()->create_component<Engine::WeaponComponent>(entity_id, COMPONENT_WEAPON);
		}

		//TODO  other components
		else if (component_type == "")
		{

		}

		else if (component_type == " ")
		{

		}

		else if (component_type == " ")
		{

		}

		else if (component_type == " ")
		{

		}
	}


	int getNumCollidedEntities(unsigned int id)
	{
		// Get collision component.
		CollisionSphereComponent* sphere = ECSManager::Instance()->get_component<CollisionSphereComponent>(id, COMPONENT_COLLISION_SPHERE);
		if (sphere == nullptr) return -1;
		return sphere->mNumOverlaps;

	} // getNumCollidedEntities()

	int getCollidedEntity(unsigned int id, unsigned int collisionIndex)
	{

		// Get collision component.
		CollisionSphereComponent* sphere = ECSManager::Instance()->get_component<CollisionSphereComponent>(id, COMPONENT_COLLISION_SPHERE);
		if (sphere == nullptr) return -1;

		return sphere->mOverlapEntityIds[collisionIndex];

	} // getCollidedEntities()	

}