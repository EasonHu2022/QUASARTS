#include "ECS/ECSManager.h"

namespace Engine {
    // Constructor:
    ECSManager::ECSManager() {
        entity_IDs = {0};
    }

    // Destructor:
    ECSManager::~ECSManager() {

    }

    // Get a new entity ID (the first free one):
    unsigned int ECSManager::get_free_entity_ID() {
        // Returns the position of the first free entity.
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
    bool ECSManager::create_entity() {
        // Get a new entity ID:
        unsigned int entityID = get_free_entity_ID();
        // Check that the new ID is valid:
        if (entityID == (MAX_ENTITIES + 1)) {
            std::cerr << "Warning: unable to create new entity. Maximum number of entities reached." << std::endl;
            return false;
        }
        Entity new_entity = Entity(entityID);
        entities.push_back(new_entity);
        entity_ID_match.push_back(entityID);
        return true;
    }

    // Version of create_entity() with a component mask:
    bool ECSManager::create_entity(quasarts_component_mask mask) {
        // Get a new entity ID:
        unsigned int entityID = get_free_entity_ID();
        // Check that the new ID is valid:
        if (entityID == (MAX_ENTITIES + 1)) {
            std::cerr << "Warning: unable to create new entity. Maximum number of entities reached." << std::endl;
            return false;
        }
        Entity new_entity = Entity(entityID);
        new_entity.set_component_mask(mask);
        entities.push_back(new_entity);
        entity_ID_match.push_back(entityID);

        // Process component mask and add data entries for components in component arrays:
        unsigned int bit;
        for (int i = 0; i < MAX_COMPONENT_TYPES; i++) {
            bit = 1 << i;
            if ((mask.mask & bit) == bit) {
                // The entity has this component type:
                componentArrays[i]->add_data(entityID);
            }
        }

        // Update Systems:
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->test_entity(mask, entityID);
        }

        return true;
    }

    // Create a new entity by copying another:
    bool ECSManager::copy_entity(unsigned int copyID) {
        // Get a new entity ID:
        unsigned int entityID = get_free_entity_ID();
        // Check that the new ID is valid:
        if (entityID == (MAX_ENTITIES + 1)) {
            std::cerr << "Warning: unable to create new entity. Maximum number of entities reached." << std::endl;
            return false;
        }
        Entity new_entity = Entity(entityID);

        // Find index of entity to be copied:
        int matched = 0;
        unsigned int copy_from;
        for (int i = 0; i < entity_ID_match.size(); i++) {
            if (entity_ID_match[i] == copyID) {
                copy_from = i;
                matched = 1;
                break;
            }
        }
        // If no match was found, print a warning and leave:
        if (matched == 0) {
            std::cerr << "Warning: no match was found for entity " << entityID << "!" << std::endl;
            return false;
        }

        new_entity.set_component_mask(entities[copy_from].get_componentMask());
        entities.push_back(new_entity);
        entity_ID_match.push_back(entityID);

        // Update Systems:
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->test_entity(new_entity.get_componentMask(), entityID);
        }

        // Copy data from one Entity to the other:
        unsigned int bit;
        for (int i = 0; i < MAX_COMPONENT_TYPES; i++) {
            bit = 1 << i;
            if ((new_entity.get_componentMask().mask & bit) == bit) {
                // The entity has this component type:
                componentArrays[i]->copy_data(copy_from, entityID);
            }
        }
        return true;
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
            componentArrays[i]->remove_data(entityID);
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

    // Remove a Component from an Entity:
    void ECSManager::destroy_component(unsigned int entityID, unsigned int componentType) {
        // Update the component mask of the entity:
        int matched = 0;
        unsigned int entity_index;
        for (int i = 0; i < entity_ID_match.size(); i++) {
            if (entity_ID_match[i] == entityID) {
                entities[i].remove_component_type(componentType);
                entity_index = i;
                matched = 1;
                break;
            }
        }
        // If no match was found, print a warning and leave:
        if (matched == 0) {
            std::cerr << "Warning: no match was found for entity " << entityID << "!" << std::endl;
            return;
        }
        // Update each System:
        for (int i = 0; i < systems.size(); i++) {
            systems[i]->test_entity(entities[entity_index].get_componentMask(), entityID);
        }
        // Remove the data from the relevant component array:
        componentArrays[componentType]->remove_data(entityID);
    }
}