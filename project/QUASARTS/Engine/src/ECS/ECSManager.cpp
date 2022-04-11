#include "ECS/ECSManager.h"

namespace Engine {
    // Constructor:
    ECSManager::ECSManager() {
        entity_IDs = {0};

        // Add Component arrays:
        ComponentArray<TransformComponent> *array_transform = new ComponentArray<TransformComponent>;
        ComponentArray<MeshComponent> *array_mesh = new ComponentArray<MeshComponent>;

        componentArrays.push_back(array_transform);
        componentArrays.push_back(array_mesh);

        // Add Systems:
        sys_example = new ExampleSystem();

        systems.push_back(sys_example);
    }

    // Destructor:
    ECSManager::~ECSManager() {
        // Destroy Component arrays:
        for (int i = 0; i < componentArrays.size(); i++) {
            delete componentArrays[i];
        }
    }

    // Get a new Entity ID (the first free one):
    unsigned int ECSManager::get_free_entity_ID() {
        // Returns the position of the first free Entity.
        // DOES NOT write into the array.
        for (unsigned int i = 0; i < MAX_ENTITIES; i++) {
            if (entity_IDs.mask[i] == 0) {
                return i;
            }
        }
        // If we get here then there are no free IDs - return an invalid value:
        return MAX_ENTITIES + 1;
    }

    // Create a new Entity and add it to the vector:
    unsigned int ECSManager::create_entity() {
        // Get a new entity ID:
        unsigned int entityID = get_free_entity_ID();
        // Check that the new ID is valid:
        if (entityID == (MAX_ENTITIES + 1)) {
            std::cerr << "Warning: unable to create new entity. Maximum number of entities reached." << std::endl;
            return entityID;
        }

        // Create the Entity:
        Entity new_entity = Entity(entityID);

        // Update the manager entries:
        entities.push_back(new_entity);
        entity_ID_match.push_back(entityID);
        entity_IDs.mask[entityID] = 1;
        return entityID;
    }

    // Destroy an Entity:
    void ECSManager::destroy_entity(unsigned int entityID) {
        /* Tasks:
         * Free up the Entity ID.
         * Remove all Component data.
         * Rearrange the Component data so that it's packed.
         * Change the entity mask in all Systems.
         * Remove the Entity from the vector. */

        // 1. Free up the Entity ID:
        entity_IDs.mask[entityID] = 0;

        // 2, 3. Remove all Component data and rearrange:
        for (int i = 0; i < componentArrays.size(); i++) {
            componentArrays[i]->remove_entity(entityID);
        }

        // 4. Change the entity mask in all Systems:
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->clear_entity(entityID);
        }

        // 5. Remove the Entity from the vector:
        for (int i = 0; i < entity_ID_match.size(); i++) {
            if (entity_ID_match[i] == entityID) {
                entities.erase(entities.begin() + i);
                entity_ID_match.erase(entity_ID_match.begin() + i);
                break;
            }
        }
    }

    // Get a pointer to an Entity:
    Entity *ECSManager::get_entity(unsigned int entityID) {
        for (int i = 0; i < entity_ID_match.size(); i++) {
            if (entity_ID_match[i] == entityID) {
                return &entities[i];
            }
        }
        return NULL;
    }

    // Get the list of Entity IDs:
    quasarts_entity_ID_mask *ECSManager::get_entityIDs() {
        return &entity_IDs;
    }

    // Save the whole scene to file:
    bool ECSManager::save_scene(char *filename) {
        /* I'm thinking that the file could contain:
         * - Entity ID mask detailing all Entity IDs in use.
         * - Component masks for each Entity.
         * - Data for Components - Component arrays.
         * - Entity ID matches for Component array elements. */
        return true;
    }

    // Load a scene from file:
    bool ECSManager::load_scene(char *filename) {
        return true;
    }

    // Print Entity information for debugging purposes:
    void ECSManager::print_entities() {
        for (int i = 0; i < entities.size(); i++) {
            Entity entity = entities[i];
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
        componentArrays[componentType]->print_state();
    }
}