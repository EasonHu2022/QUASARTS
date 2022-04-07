#pragma once

// Library includes:
#include <vector>
#include <iostream>

// Local includes:
#include "Core/Core.h"
#include "ECS/ECS-Common.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/ComponentArray.h"
#include "ECS/System/System.h"

namespace Engine {
    class QS_API ECSManager {
        public:
        // Constructor and destructor:
        ECSManager();
        ~ECSManager();

        // Get a new entity ID (the first free one):
        unsigned int get_free_entity_ID();

        // Create a new Entity and add it to the vector:
        bool create_entity();

        // Version of create_entity() with a component mask:
        bool create_entity(quasarts_component_mask mask);

        // Create a new entity by copying another:
        bool copy_entity(unsigned int copyID);

        // Destroy an Entity:
        void destroy_entity(unsigned int entityID);

        // Create a new Component and add it to a specific Entity:
        template <typename T>
        void create_component(unsigned int entityID, unsigned int componentType, T data) {
            // Update the component mask of the entity:
            int matched = 0;
            unsigned int entity_index;
            for (int i = 0; i < entity_ID_match.size(); i++) {
                if (entity_ID_match[i] == entityID) {
                    entities[i].add_component_type(componentType);
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
            // Add the data to the relevant component array:
            componentArrays[componentType]->add_data(data, entityID);
        }

        // Remove a Component from an Entity:
        void destroy_component(unsigned int entityID, unsigned int componentType);

        private:
        // Entities:
        std::vector<Entity> entities;
        std::vector<unsigned int> entity_ID_match;

        // Component arrays:
        std::vector<ParentComponentArray *> componentArrays;

        // Systems:
        std::vector<System *> systems;  // For iterating over Systems easily.

        // Bit mask (array) of entity IDs that are in use (0 = free, 1 = used):
        quasarts_entity_ID_mask entity_IDs;
    };
}