#include "ECS/ECSManager.h"

namespace Engine {
    // Singleton:
	ECSManager *ECSManager::instance = nullptr;

	// Create the instance of the ECSManager:
	ECSManager *ECSManager::Instance() {
		if (nullptr == instance)
			instance = new ECSManager();
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
            std::cerr << "Warning: unable to create new entity. Maximum number of entities reached." << std::endl;
            return entityID;
        }

        // Create the Entity:
        Entity new_entity = Entity(entityID);

        // Update the manager entries:
        scene->entities.push_back(new_entity);
        scene->entity_ID_match.push_back(entityID);
        scene->entity_IDs.mask[entityID] = 1;
        return entityID;
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

        // Remove the Entity from the vector:
        for (int i = 0; i < scene->entity_ID_match.size(); i++) {
            if (scene->entity_ID_match[i] == entityID) {
                scene->entities.erase(scene->entities.begin() + i);
                scene->entity_ID_match.erase(scene->entity_ID_match.begin() + i);
                break;
            }
        }
    }

    // Get a pointer to an Entity:
    Entity *ECSManager::get_entity(unsigned int entityID) {
        for (int i = 0; i < scene->entity_ID_match.size(); i++) {
            if (scene->entity_ID_match[i] == entityID) {
                return &(scene->entities[i]);
            }
        }
        return NULL;
    }

    // Get the list of Entity IDs:
    quasarts_entity_ID_mask *ECSManager::get_entityIDs() {
        return &(scene->entity_IDs);
    }

    // Register a System with the Manager:
    void ECSManager::register_system(unsigned int systemType, System *system) {
        systems[systemType] = system;
    }

    // Deregister a System with the Manager:
    void ECSManager::deregister_system(unsigned int systemType) {
        systems.erase(systemType);
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

    // Print Entity information for debugging purposes:
    void ECSManager::print_entities() {
        for (int i = 0; i < scene->entities.size(); i++) {
            Entity entity = scene->entities[i];
            std::cout << "Entity ID: " << entity.get_entityID() << std::endl;
            std::cout << "Component Mask:" << std::endl;
            quasarts_component_mask mask;
            for (int j = MAX_COMPONENT_TYPES - 1; j >= 0 ; j--) {
                mask.mask = (uint64_t)1 << j;
                if ((entity.get_componentMask().mask & mask.mask) == mask.mask) {
                    std::cout << "1";
                } else { std::cout << "0"; }
            }
            std::cout << std::endl;
        }
    }

    // Print out Component array information:
    void ECSManager::print_componentArray_info(unsigned int componentType) {
        scene->componentArrays[componentType]->print_state();
    }
}