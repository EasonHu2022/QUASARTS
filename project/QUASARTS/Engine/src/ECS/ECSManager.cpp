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
        // Get a new entity ID:
        unsigned int entityID = get_free_entity_ID();
        // Check that the new ID is valid:
        if (entityID == TOO_MANY_ENTITIES) {
            std::cerr << "Function ECSManager::create_entity(): Warning: \
                            unable to create new entity. Maximum number of \
                            entities reached." << std::endl;
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

        // Change the entity mask in all Systems:
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->clear_entity(entityID);
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

    // Add an entity group:
    void ECSManager::add_entity_group(std::string group_name) {
        if (scene->entity_groups.find(group_name) != scene->entity_groups.end()) {
            std::cerr << "Function ECSManager::add_entity_group: Warning: group \
                                                    already exists!" << std::endl;
            return;
        }
        scene->entity_groups[group_name] = {};
    }

    // Destroy an entity group (does not destroy entities in the group):
    void ECSManager::destroy_entity_group(std::string group_name) {
        scene->entity_groups.erase(group_name);
    }

    // Add an entity to a group:
    void ECSManager::add_entity_to_group(std::string group_name, unsigned int entityID) {
        scene->entity_groups[group_name].emplace(entityID);
    }

    // Remove an entity from a group:
    void ECSManager::remove_entity_from_group(std::string group_name, unsigned int entityID) {
        scene->entity_groups[group_name].erase(entityID);
    }

    // Destroy all the entities in a group (does not destroy the group):
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
        if (index == TOO_MANY_ENTITIES) { return NULL; }
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
            std::cerr << "Function ECSManager::add_child: Warning: no match \
                                    was found for parent entity " << parent <<
                                    "!" << std::endl;
            return;
        } else if (child_index == TOO_MANY_ENTITIES) {
            std::cerr << "Function ECSManager::add_child: Warning: no match \
                                    was found for child entity " << child <<
                                    "!" << std::endl;
            return;
        }

        // Take into account the situation where a child already has a parent:
        if (has_parent(child_index) == true) {
            std::cerr << "Function ECSManager::add_child: Warning: child \
                                    entity " << child << " already has a \
                                    parent! Creation failed." << std::endl;
            return;
        }

        // Deal with the situation where the child is the parent of the parent:
        if (get_parent(parent_index) == child) {
            std::cerr << "Function ECSManager::add_child: Warning: child \
                                    entity " << child << " is the parent of \
                                    parent entity " << parent << "! Circular \
                                    relationship forbidden, creation failed."
                                    << std::endl;
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
            std::cerr << "Function ECSManager::remove_child: Warning: no match \
                                    was found for parent entity " << parent <<
                                    "!" << std::endl;
            return;
        } else if (child_index == TOO_MANY_ENTITIES) {
            std::cerr << "Function ECSManager::remove_child: Warning: no match \
                                    was found for child entity " << child <<
                                    "!" << std::endl;
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
            std::cerr << "Function ECSManager::get_children: Warning: no match \
                        was found for entity " << entityID << "!" << std::endl;
            return {};
        }
        return scene->children[index];
    }

    // Get the parent of an Entity:
    unsigned int ECSManager::get_parent(unsigned int entityID) {
        unsigned int index = get_index_from_ID(entityID);
        if (index == TOO_MANY_ENTITIES) {
            std::cerr << "Function ECSManager::get_parent: Warning: no match \
                        was found for entity " << entityID << "!" << std::endl;
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

    // Get the name of the current scene:
    std::string ECSManager::get_scene_name() {
        return scene->name;
    }

    // Set the pointer to the current scene:
    void ECSManager::set_scene(Scene *scene_ptr) {
        scene = scene_ptr;
    }

    // Save the whole scene to file:
    bool ECSManager::save_scene(char *filename) {
        return true;
    }

    // Load a scene from file:
    bool ECSManager::load_scene(char *filename) {
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
            // Print Entity ID:
            Entity entity = scene->entities[i];
            std::cout << "Entity ID: " << entity.get_entityID() << std::endl;

            // Print Component mask:
            std::cout << "Component Mask:" << std::endl;
            quasarts_component_mask mask;
            for (int j = MAX_COMPONENT_TYPES - 1; j >= 0 ; j--) {
                mask.mask = (uint64_t)1 << j;
                if ((entity.get_componentMask()->mask & mask.mask) == mask.mask) {
                    std::cout << "1";
                } else { std::cout << "0"; }
            }
            std::cout << std::endl;

            // Print parent-child relationships of Entity:
            std::cout << "Parent: ";
            if (scene->parents[i] == TOO_MANY_ENTITIES) {
                std::cout << "No parent" << std::endl;
            } else {
                std::cout << scene->parents[i] << std::endl;
            }
            std::cout << "Children:" << std::endl;
            std::set<unsigned int>::iterator iter;
            for (iter = scene->children[i].begin();
                            iter != scene->children[i].end(); iter++) {
                std::cout << *iter << ", ";
            }
            std::cout << std::endl;

            // Print name of Entity:
            std::cout << "Entity name: " << entity.get_name() << std::endl;
        }
    }

    // Print out Component array information:
    void ECSManager::print_componentArray_info(unsigned int componentType) {
        scene->componentArrays[componentType]->print_state();
    }
}