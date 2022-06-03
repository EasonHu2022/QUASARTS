#include "ECS/ECSManager.h"

namespace Engine {
    // Singleton:
	ECSManager *ECSManager::instance = nullptr;

	// Create the instance of the ECSManager:
	ECSManager *ECSManager::Instance() {
		if (nullptr == instance) {
			instance = new ECSManager();
        }
		return instance;
	}

    void ECSManager::init() {

    }

    int ECSManager::start() {
        return 0;
    }

    void ECSManager::update() {

    }

    int ECSManager::stop() {
        return 0;
    }

    void ECSManager::release() {

    }

    // Get a new Entity ID (the first free one):
    unsigned int ECSManager::get_free_entity_ID() {
        // Returns the position of the first free Entity.
        // DOES NOT write into the array.
        for (unsigned int i = 0; i < MAX_ENTITIES; i++) {
            if (scene->entity_IDs.mask[i] == 0) {
                return i;
            }
        }
        // If we get here then there are no free IDs - return an invalid value:
        return TOO_MANY_ENTITIES;
    }

    // Create a new Entity and add it to the vector:
    unsigned int ECSManager::create_entity() {
        // Get a new Entity ID:
        unsigned int entityID = get_free_entity_ID();
        // Check that the new ID is valid:
        if (entityID == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::create_entity(): Error: unable to create new Entity. Maximum number of Entities reached.");
            return entityID;
        }

        // Create the Entity:
        Entity new_entity = Entity(entityID);

        // Update the manager entries:
        scene->entities.push_back(new_entity);
        scene->entity_ID_match.push_back(entityID);
        scene->entity_IDs.mask[entityID] = 1;
        scene->parents.push_back(TOO_MANY_ENTITIES);
        scene->children.push_back({});

        // Add a transform Component to the Entity:
        create_component<TransformComponent>(entityID, COMPONENT_TRANSFORM);

        return entityID;
    }

    // Get the name of an Entity:
    std::string ECSManager::get_entityName(unsigned int entityID) {
        Entity *entity = get_entity(entityID);
        return entity->get_name();
    }

    // Change the name of an Entity:
    void ECSManager::set_entityName(unsigned int entityID, std::string name) {
        Entity *entity = get_entity(entityID);
        std::string new_name = name + "##" + std::to_string(entityID);
        entity->set_name(new_name);
    }

    // Destroy an Entity:
    void ECSManager::destroy_entity(unsigned int entityID) {
        // Free up the Entity ID:
        scene->entity_IDs.mask[entityID] = 0;

        // Remove all Component data and rearrange:
        for (int i = 0; i < scene->componentArrays.size(); i++) {
            scene->componentArrays[i]->remove_entity(entityID);
        }

        // Change the Entity mask in all Systems:
        for (const auto &[key, val] : systems) {
            val->clear_entity(entityID);
        }

        // Remove the Entity from the manager:
        unsigned int index = get_index_from_ID(entityID);
        if (index == TOO_MANY_ENTITIES) { return; }
        scene->entities.erase(scene->entities.begin() + index);
        scene->entity_ID_match.erase(scene->entity_ID_match.begin() + index);
        scene->parents.erase(scene->parents.begin() + index);
        scene->children.erase(scene->children.begin() + index);

        // Remove all references to the Entity from relationships:
        for (int i = 0; i < scene->entities.size(); i++) {
            if (scene->parents[i] == entityID) {
                scene->parents[i] = TOO_MANY_ENTITIES;
            }
            scene->children[i].erase(entityID);
        }
    }

    // Set the value of game_running:
    void ECSManager::set_game_running(bool state) {
        game_running = state;
    }

    // Get the value of game_running:
    bool ECSManager::get_game_running() {
        return game_running;
    }

    // Get the number of entities in the scene:
    unsigned int ECSManager::get_num_entities() {
        return (unsigned int)(scene->entities.size());
    }

    // Check if an Entity has a particular type of Component:
    bool ECSManager::has_component(unsigned int entityID, unsigned int componentType) {
        Entity *entity = get_entity(entityID);
        quasarts_component_mask *entity_mask = entity->get_componentMask();
        quasarts_component_mask test_mask{};
        test_mask.mask = (uint64_t)1 << componentType;
        if ((test_mask.mask & entity_mask->mask) == test_mask.mask) {
            return true;
        }
        return false;
    }

    // Get all Component types that an Entity has:
    std::vector<unsigned int> ECSManager::get_all_component_types(unsigned int entityID) {
        std::vector<unsigned int> result = {};
        for (unsigned int i = 0; i < NUM_COMPONENT_TYPES; i++) {
            if (has_component(entityID, i)) {
                result.push_back(i);
            }
        }
        return result;
    }

    // Add an Entity group:
    void ECSManager::add_entity_group(std::string group_name) {
        if (scene->entity_groups.find(group_name) != scene->entity_groups.end()) {
            QERROR("Function ECSManager::add_entity_group: Error: group already exists!");
            return;
        }
        scene->entity_groups[group_name] = {};
    }

    // Destroy an Entity group (does not destroy Entities in the group):
    void ECSManager::destroy_entity_group(std::string group_name) {
        scene->entity_groups.erase(group_name);
    }

    // Add an Entity to a group:
    void ECSManager::add_entity_to_group(std::string group_name, unsigned int entityID) {
        scene->entity_groups[group_name].emplace(entityID);
    }

    // Remove an Entity from a group:
    void ECSManager::remove_entity_from_group(std::string group_name, unsigned int entityID) {
        scene->entity_groups[group_name].erase(entityID);
    }

    // Destroy all the Entities in a group (does not destroy the group):
    void ECSManager::destroy_entities_in_group(std::string group_name) {
        std::set<unsigned int>::iterator iter;
        for (iter = scene->entity_groups[group_name].begin();
                    iter != scene->entity_groups[group_name].end(); iter++) {
            destroy_entity(*iter);
        }
        scene->entity_groups[group_name].clear();
    }

    // Get a pointer to an Entity:
    Entity *ECSManager::get_entity(unsigned int entityID) {
        unsigned int index = get_index_from_ID(entityID);
        if (index == TOO_MANY_ENTITIES) { return nullptr; }
        return &(scene->entities[index]);
    }

    // Get the mask of Entity IDs:
    quasarts_entity_ID_mask ECSManager::get_entity_ID_mask() {
        return scene->entity_IDs;
    }

    // Get the Entity ID match list:
    std::vector<unsigned int> ECSManager::get_entity_ID_match() {
        return scene->entity_ID_match;
    }

    // Add a child to an Entity:
    void ECSManager::add_child(unsigned int parent, unsigned int child) {
        // Check that both parent and child exist:
        unsigned int parent_index = get_index_from_ID(parent);
        unsigned int child_index = get_index_from_ID(child);
        if (parent_index == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::add_child: Error: no match was found for parent Entity {0}!", parent);
            return;
        } else if (child_index == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::add_child: Error: no match was found for child Entity {0}!", child);
            return;
        }

        // Take into account the situation where a child already has a parent:
        if (has_parent(child_index) == true) {
            QERROR("Function ECSManager::add_child: Error: child Entity {0} already has a parent!", child);
            return;
        }

        // Deal with the situation where the child is the parent of the parent:
        if (get_parent(parent_index) == child) {
            QERROR("Function ECSManager::add_child: Error: child Entity {0} is the parent of parent Entity {1}!", child, parent);
            return;
        }

        // If both parent and child exist, create the relationship:
        scene->children[parent_index].emplace(child);
        scene->parents[child_index] = parent;
    }

    // Remove a child from an Entity:
    void ECSManager::remove_child(unsigned int parent, unsigned int child) {
        // Check that both parent and child exist:
        unsigned int parent_index = get_index_from_ID(parent);
        unsigned int child_index = get_index_from_ID(child);
        if (parent_index == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::remove_child: Error: no match was found for parent Entity {0}!", parent);
            return;
        } else if (child_index == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::remove_child: Error: no match was found for child Entity {0}!", child);
            return;
        }

        // If both parent and child exist, remove the relationship:
        scene->children[parent_index].erase(child);
        scene->parents[child_index] = TOO_MANY_ENTITIES;
    }

    // Get the children of an Entity:
    std::set<unsigned int> ECSManager::get_children(unsigned int entityID) {
        unsigned int index = get_index_from_ID(entityID);
        if (index == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::get_children: Error: no match was found for Entity {0}!", entityID);
            return {};
        }
        return scene->children[index];
    }

    // Get the parent of an Entity:
    unsigned int ECSManager::get_parent(unsigned int entityID) {
        unsigned int index = get_index_from_ID(entityID);
        if (index == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::get_parent: Error: no match was found for Entity {0}!", entityID);
            return TOO_MANY_ENTITIES;
        }
        return scene->parents[index];
    }

    // Check if the Entity has a parent:
    bool ECSManager::has_parent(unsigned int entityID) {
        if (get_parent(entityID) == TOO_MANY_ENTITIES) {
            return false;
        }
        return true;
    }

    // Check if the Entity has children:
    bool ECSManager::has_children(unsigned int entityID) {
        if (get_children(entityID).size() == 0) {
            return false;
        }
        return true;
    }

    // Register a System with the Manager:
    void ECSManager::register_system(unsigned int systemType, System *system) {
        systems[systemType] = system;
        system->set_manager(this);
    }

    // Deregister a System with the Manager:
    void ECSManager::deregister_system(unsigned int systemType) {
        systems.erase(systemType);
    }

    // Get the current Entity ID:
    unsigned int ECSManager::get_current_entity() {
        return current_entity;
    }

    // Set the current Entity ID:
    void ECSManager::set_current_entity(unsigned int entityID) {
        current_entity = entityID;
    }

    // Create the scene camera:
    void ECSManager::create_camera() {
        // Create the camera:
        unsigned int cameraID = get_free_entity_ID();
        if (cameraID == TOO_MANY_ENTITIES) {
            QERROR("Function ECSManager::create_camera(): Error: unable to create new Entity. Maximum number of Entities reached.");
            return;
        }
        scene->create_camera(cameraID);

        // Update each System:
        unsigned int index = get_index_from_ID(cameraID);
        for (const auto &[key, val] : systems) {
            val->test_entity(this, cameraID, scene->entities[index].get_componentMask());
        }
    }

    // Get the scene camera:
    unsigned int ECSManager::get_camera() {
        return scene->camera;
    }

    // Set the scene camera:
    void ECSManager::set_camera(unsigned int cameraID) {
        scene->camera = cameraID;
    }

    // Get the name of the current scene:
    std::string ECSManager::get_scene_name() {
        return scene->name;
    }

    // Set the name of the scene:
    void ECSManager::set_scene_name(std::string name) {
        scene->name = name;
    }

    // Create new blank scene:
    void ECSManager::new_scene() {
        // Clear the scene data:
        scene->clear_data();

        // Clear the System entity masks:
        for (const auto &[key, val] : systems) {
            val->clear_all_entity_masks();
        }

        // Recreate the camera:
        create_camera();

        // Reset the current entity:
        set_current_entity(TOO_MANY_ENTITIES);

        // Initialize appropriate Components:
        for (const auto &[key, val] : systems) {
                val->initialize_components();
        }

        QDEBUG("New scene created.");
    }

    // Create new blank scene with a name:
    void ECSManager::new_scene(std::string name) {
        // Clear the scene data:
        scene->clear_data();

        // Clear the System entity masks:
        for (const auto &[key, val] : systems) {
            val->clear_all_entity_masks();
        }

        // Set the scene name:
        scene->name = name;

        // Recreate the camera:
        create_camera();

        // Reset the current entity:
        set_current_entity(TOO_MANY_ENTITIES);

        // Initialize appropriate Components:
        for (const auto &[key, val] : systems) {
                val->initialize_components();
        }

        QDEBUG("New scene created.");
    }

    // Save the whole scene to file:
    bool ECSManager::save_scene(char *filename) {
        std::ofstream sceneFile(filename);
        // Check if the file is open:
        if (!(sceneFile.good())) {
            QERROR("Function ECSManager::save_scene: Error: Cannot open file for writing!");
            return false;
        }

        // Write the header:
        sceneFile << "# Quasarts Orbit Engine Scene" << std::endl;
        sceneFile << "# N = Name" << std::endl;
        sceneFile << "# E = Entity (followed by ID, a flag to indicate scene camera, and name)" << std::endl;
        sceneFile << "# C = Component (followed by Entity ID, Component type and data)" << std::endl;
        sceneFile << "# P = Parent-child (entity ID of parent, then a list of children)" << std::endl;

        // Write the scene data. Start with the name:
        sceneFile << "N " << scene->name << std::endl;

        // Entities with their IDs and names:
        for (int i = 0; i < scene->entities.size(); i++) {
            sceneFile << "E " << scene->entities[i].get_entityID() << " ";
            // Check if the Entity is the scene camera:
            if (get_camera() == scene->entities[i].get_entityID()) {
                sceneFile << 1 << " ";
            } else {
                sceneFile << 0 << " ";
            }
                sceneFile << scene->entities[i].get_name() << std::endl;
        }

        // Components:
        for (int i = 0; i < scene->entities.size(); i++) {
            unsigned int entityID = scene->entities[i].get_entityID();
            // Cycle through Component types:
            if (has_component(entityID, COMPONENT_TRANSFORM) == true) {
                sceneFile << "C " << entityID << " ";
                TransformComponent *transform = get_component<TransformComponent>
                                                (entityID, COMPONENT_TRANSFORM);
                sceneFile << COMPONENT_TRANSFORM << " " << *transform << std::endl;
            }
            if (has_component(entityID, COMPONENT_MESH) == true) {
                sceneFile << "C " << entityID << " ";
                MeshComponent *mesh = get_component<MeshComponent>
                                                (entityID, COMPONENT_MESH);
                sceneFile << COMPONENT_MESH << " " << *mesh << std::endl;
            }
            if (has_component(entityID, COMPONENT_COLLISION_SPHERE) == true) {
                sceneFile << "C " << entityID << " ";
                CollisionSphereComponent *collisionSphere = get_component<CollisionSphereComponent>
                                                (entityID, COMPONENT_COLLISION_SPHERE);
                sceneFile << COMPONENT_COLLISION_SPHERE << " " << *collisionSphere << std::endl;
            }
            if (has_component(entityID, COMPONENT_MATERIAL) == true) {
                sceneFile << "C " << entityID << " ";
                MaterialComponent *material = get_component<MaterialComponent>
                                                (entityID, COMPONENT_MATERIAL);
                sceneFile << COMPONENT_MATERIAL << " " << *material << std::endl;
            }
            if (has_component(entityID, COMPONENT_LIGHTING) == true) {
                sceneFile << "C " << entityID << " ";
                LightComponent *light = get_component<LightComponent>
                                                (entityID, COMPONENT_LIGHTING);
                sceneFile << COMPONENT_LIGHTING << " " << *light << std::endl;
            }
            if (has_component(entityID, COMPONENT_SCRIPT) == true) {
                sceneFile << "C " << entityID << " ";
                ScriptComponent *script = get_component<ScriptComponent>
                                                (entityID, COMPONENT_SCRIPT);
                sceneFile << COMPONENT_SCRIPT << " " << *script << std::endl;
            }
            if (has_component(entityID, COMPONENT_CAMERA) == true) {
                sceneFile << "C " << entityID << " ";
                CameraComponent *camera = get_component<CameraComponent>
                                                (entityID, COMPONENT_CAMERA);
                sceneFile << COMPONENT_CAMERA << " " << *camera << std::endl;
            }
            if (has_component(entityID, COMPONENT_ORBIT) == true) {
                sceneFile << "C " << entityID << " ";
                OrbitComponent *orbit = get_component<OrbitComponent>
                                                (entityID, COMPONENT_ORBIT);
                sceneFile << COMPONENT_ORBIT << " " << *orbit << std::endl;
            }
            if (has_component(entityID, COMPONENT_HEALTH) == true) {
                sceneFile << "C " << entityID << " ";
                HealthComponent *health = get_component<HealthComponent>
                                                (entityID, COMPONENT_HEALTH);
                sceneFile << COMPONENT_HEALTH << " " << *health << std::endl;
            }
            if (has_component(entityID, COMPONENT_WEAPON) == true) {
                sceneFile << "C " << entityID << " ";
                WeaponComponent *weapon = get_component<WeaponComponent>
                                                (entityID, COMPONENT_WEAPON);
                sceneFile << COMPONENT_WEAPON << " " << *weapon << std::endl;
            }
            if (has_component(entityID, COMPONENT_PARTICLE) == true) {
                sceneFile << "C " << entityID << " ";
                ParticleComponent *particle = get_component<ParticleComponent>
                                                (entityID, COMPONENT_PARTICLE);
                sceneFile << COMPONENT_PARTICLE << " " << *particle << std::endl;
            }
            if (has_component(entityID, COMPONENT_ENEMY) == true) {
                sceneFile << "C " << entityID << " ";
                EnemyComponent *enemy = get_component<EnemyComponent>
                                                (entityID, COMPONENT_ENEMY);
                sceneFile << COMPONENT_ENEMY << " " << *enemy << std::endl;
            }
            if (has_component(entityID, COMPONENT_ENEMY_SPAWNER) == true) {
                sceneFile << "C " << entityID << " ";
                EnemySpawnComponent *enemySpawner = get_component<EnemySpawnComponent>
                                                (entityID, COMPONENT_ENEMY_SPAWNER);
                sceneFile << COMPONENT_ENEMY_SPAWNER << " " << *enemySpawner << std::endl;
            }
            if (has_component(entityID, COMPONENT_PROJECTILE) == true) {
                sceneFile << "C " << entityID << " ";
                ProjectileComponent *projectile = get_component<ProjectileComponent>
                                                (entityID, COMPONENT_PROJECTILE);
                sceneFile << COMPONENT_PROJECTILE << " " << *projectile << std::endl;
            }
            if (has_component(entityID, COMPONENT_AUDIO) == true) {
                sceneFile << "C " << entityID << " ";
                AudioComponent *audio = get_component<AudioComponent>
                                                (entityID, COMPONENT_AUDIO);
                sceneFile << COMPONENT_AUDIO << " " << *audio << std::endl;
            }
        }

        // Parent-child relationships:
        for (int i = 0; i < scene->parents.size(); i++) {
            if (scene->parents[i] == TOO_MANY_ENTITIES) { continue; }
            sceneFile << "P " << scene->parents[i];
            std::set<unsigned int>::iterator iter;
            for (iter = scene->children[scene->parents[i]].begin();
                    iter != scene->children[scene->parents[i]].end(); iter++) {
                sceneFile << " " << *iter;
            }
            sceneFile << std::endl;
        }

        QDEBUG("Scene \"{0}\" saved.", scene->name);
        return true;
    }

    // Load a scene from file:
    bool ECSManager::load_scene(char *filename) {
        // This will load the contents of the file into a new Scene object.
        std::ifstream sceneFile(filename);
        // Check if the file is open:
        if (!(sceneFile.good())) {
            QERROR("Function ECSManager::load_scene: Error: Cannot open file for reading!");
            return false;
        }

        // Clear the scene data:
        scene->clear_data();

        // Clear the System entity masks:
        for (const auto &[key, val] : systems) {
            val->clear_all_entity_masks();
        }

        // Reset the current entity:
        set_current_entity(TOO_MANY_ENTITIES);

        // Create a buffer for reading in lines:
        char lineBuffer[MAX_SCENE_LINE_LENGTH];

        // Begin reading the file:
        while (true) {
            // Read the line and check for EOF:
            sceneFile.getline(lineBuffer, MAX_SCENE_LINE_LENGTH);
            std::string line = std::string(lineBuffer);
            if (sceneFile.eof()) { break; }

            // Test first character:
            // COMMENT //
            if (line[0] == '#') {
                continue;
            // NAME //
            } else if (line[0] == 'N') {
                scene->name = line.substr(2, line.length());
            // ENTITY //
            } else if (line[0] == 'E') {
                std::stringstream parser(line.substr(2, line.length()));
                unsigned int entityID;
                unsigned int cameraFlag;
                std::string name;
                std::string tmp;
                parser >> entityID;
                parser >> cameraFlag;
                parser >> name;
                while (parser >> tmp) {
                    name = name + " " + tmp;
                }
                if (cameraFlag == 1) {
                    // This is the camera:
                    set_camera(entityID);
                }
                Entity new_entity = Entity(entityID);
                new_entity.set_name(name);
                scene->entities.push_back(new_entity);
                scene->entity_ID_match.push_back(entityID);
                scene->entity_IDs.mask[entityID] = 1;
                scene->parents.push_back(TOO_MANY_ENTITIES);
                scene->children.push_back({});
            // COMPONENT //
            } else if (line[0] == 'C') {
                unsigned int entityID;
                unsigned int componentType;
                std::stringstream parser(line.substr(2, line.length()));
                parser >> entityID;
                parser >> componentType;
                // Separate code for each component type:
                if (componentType == COMPONENT_TRANSFORM) {
                    TransformComponent transform{};
                    parser >> transform;
                    create_component(entityID, componentType, transform);
                } else if (componentType == COMPONENT_MESH) {
                    MeshComponent mesh{};
                    parser >> mesh;
                    create_component(entityID, componentType, mesh);
                } else if (componentType == COMPONENT_COLLISION_SPHERE) {
                    CollisionSphereComponent collisionSphere{};
                    parser >> collisionSphere;
                    create_component(entityID, componentType, collisionSphere);
                } else if (componentType == COMPONENT_MATERIAL) {
                    MaterialComponent material{};
                    parser >> material;
                    create_component(entityID, componentType, material);
                } else if (componentType == COMPONENT_LIGHTING) {
                    LightComponent light{};
                    parser >> light;
                    create_component(entityID, componentType, light);
                } else if (componentType == COMPONENT_SCRIPT) {
                    ScriptComponent script{};
                    parser >> script;
                    create_component(entityID, componentType, script);
                } else if (componentType == COMPONENT_CAMERA) {
                    CameraComponent camera{};
                    parser >> camera;
                    create_component(entityID, componentType, camera);
                } else if (componentType == COMPONENT_ORBIT) {
                    OrbitComponent orbit{};
                    parser >> orbit;
                    create_component(entityID, componentType, orbit);
                } else if (componentType == COMPONENT_HEALTH) {
                    HealthComponent health{};
                    parser >> health;
                    create_component(entityID, componentType, health);
                } else if (componentType == COMPONENT_WEAPON) {
                    WeaponComponent weapon{};
                    parser >> weapon;
                    create_component(entityID, componentType, weapon);
                } else if (componentType == COMPONENT_PARTICLE) {
                    ParticleComponent particle{};
                    parser >> particle;
                    create_component(entityID, componentType, particle);
                } else if (componentType = COMPONENT_ENEMY) {
                    EnemyComponent enemy{};
                    parser >> enemy;
                    create_component(entityID, componentType, enemy);
                } else if (componentType = COMPONENT_ENEMY_SPAWNER) {
                    EnemySpawnComponent enemySpawner{};
                    parser >> enemySpawner;
                    create_component(entityID, componentType, enemySpawner);
                } else if (componentType = COMPONENT_PROJECTILE) {
                    ProjectileComponent projectile{};
                    parser >> projectile;
                    create_component(entityID, componentType, projectile);
                } else if (componentType = COMPONENT_AUDIO) {
                    AudioComponent audio{};
                    parser >> audio;
                    create_component(entityID, componentType, audio);
                }
            // PARENT-CHILD //
            } else if (line[0] == 'P') {
                std::stringstream parser(line.substr(2, line.length()));
                unsigned int parent;
                unsigned int child;
                parser >> parent;
                while (parser >> child) {
                    scene->parents[child] = parent;
                    scene->children[parent].emplace(child);
                }
            }
        }

        // Update all the Systems:
        unsigned int index;
        for (int i = 0; i < scene->entities.size(); i++) {
            index = get_index_from_ID(scene->entity_ID_match[i]);
            for (const auto &[key, val] : systems) {
                val->test_entity(this, scene->entity_ID_match[i], scene->entities[index].get_componentMask());
            }
        }
        // Initialize appropriate Components:
        for (const auto &[key, val] : systems) {
                val->initialize_components();
        }

        QDEBUG("Scene \"{0}\" loaded.", scene->name);
        return true;
    }

    // Get an index for an Entity by ID:
    unsigned int ECSManager::get_index_from_ID(unsigned int entityID) {
        for (int i = 0; i < scene->entity_ID_match.size(); i++) {
            if (scene->entity_ID_match[i] == entityID) {
                return i;
            }
        }
        return TOO_MANY_ENTITIES;
    }

    // Print Entity information for debugging purposes:
    void ECSManager::print_entities() {
        for (int i = 0; i < scene->entities.size(); i++) {
            QDEBUG("****************************************************************");

            // Get Entity:
            Entity entity = scene->entities[i];

            // Print name of Entity:
            QDEBUG("Entity name: {0}", entity.get_name());

            // Print Entity ID:
            QDEBUG("Entity ID: {0}", entity.get_entityID());

            // Print Component mask:
            QDEBUG("Component Mask:");
            quasarts_component_mask mask;
            std::string componentMask = "";
            for (int j = MAX_COMPONENT_TYPES - 1; j >= 0 ; j--) {
                mask.mask = (uint64_t)1 << j;
                if ((entity.get_componentMask()->mask & mask.mask) == mask.mask) {
                    componentMask += "1";
                } else { componentMask += "0"; }
            }
            QDEBUG(componentMask);

            // Print parent-child relationships of Entity. Start with parent:
            unsigned int parent_ID;
            std::string parent_ID_str;
            if (scene->parents[i] == TOO_MANY_ENTITIES) {
                parent_ID_str = "No parent";
            } else {
                parent_ID = scene->parents[i];
                parent_ID_str = std::to_string(parent_ID);
            }
            QDEBUG("Parent: {0}", parent_ID_str);

            // Children:
            std::string childrenStr = "";
            std::set<unsigned int>::iterator iter_begin = scene->children[i].begin();
            std::set<unsigned int>::iterator iter_end = scene->children[i].end();
            unsigned int child_ID;
            if (scene->children[i].size() == 1) {
                child_ID = *iter_begin;
                QDEBUG("Children:");
                QDEBUG("{0}", child_ID);
            } else if (scene->children[i].size() > 1) {
                iter_end--;
                while (iter_begin != iter_end) {
                    child_ID = *iter_begin;
                    childrenStr += std::to_string(child_ID) + ", ";
                    iter_begin++;
                }
                child_ID = *iter_end;
                childrenStr += std::to_string(child_ID);
                QDEBUG("Children:");
                QDEBUG(childrenStr);
            } else {
                QDEBUG("Children: No children");
            }
        }
        QDEBUG("****************************************************************");
    }

    // Print out Component array information:
    void ECSManager::print_componentArray_info(unsigned int componentType) {
        scene->componentArrays[componentType]->print_state();
    }

    // Print out System information:
    void ECSManager::print_system_info(unsigned int systemType) {
        // Define a map of strings to aid in debugging:
        std::map<unsigned int, const char*> system_strings;
        system_strings[SYSTEM_SCRIPT] = "Script System";
        system_strings[SYSTEM_RENDER] = "Render System";
        system_strings[SYSTEM_PHYSICS] = "Physics System";
        system_strings[SYSTEM_COLLISION] = "Collision System";
        system_strings[SYSTEM_AUDIO] = "Audio System";
        system_strings[SYSTEM_ORBIT] = "Orbit System";
        system_strings[SYSTEM_PARTICLE] = "Particle System";

        // Check if the system is in the maps:
        if (system_strings.find(systemType) == system_strings.end()) {
            QDEBUG("Function ECSManager::print_(all_)system_info(): Warning: System type {0} not recognised!", systemType);
            return;
        }
        if (systems.find(systemType) == systems.end()) {
            QDEBUG("Function ECSManager::print_(all_)system_info(): Warning: {0} not registered!", system_strings[systemType]);
            return;
        }

        // Print the system information:
        QDEBUG("****************************************************************");
        QDEBUG("Printing information for {0}", system_strings[systemType]);
        systems[systemType]->print_masks();
    }

    // Print out all System information:
    void ECSManager::print_all_system_info() {
        for (const auto &[key, val] : systems) {
            unsigned int systemType = key;
            print_system_info(key);
        }
    }
}