#pragma once

// Library includes:
#include <vector>

// Local includes:
#include "Core/IManager.h"
#include "ECS/ECS-Common.h"

/* Functions:
 * virtual void update()
 * void clear_component_mask()
 * clear_entitity_mask()
 * clear_entity(unsigned int entityID)
 * add_component_type(unsigned int component_type)
 * void remove_component_type(unsigned int component_type)
 * void test_entity(quasarts_component_mask mask, unsigned int entityID)
 * ECSManager *get_manager()
 * void set_manager(ECSManager *manager_ptr)
 * quasarts_component_mask *get_component_mask()
 * quasarts_entity_ID_mask *get_entity_ID_mask() */

/* Members:
 * ECSManager *manager
 * quasarts_component_mask component_mask
 * quasarts_entity_ID_mask entity_mask */

namespace Engine {
    class QS_API ECSManager;
    class QS_API System : public IManager {
        public:
        // Constructor and destructor:
        System() : manager(nullptr), component_masks({}), entity_masks({}) {}
        ~System() {}

        // Function to add a component mask:
        void add_component_mask(quasarts_component_mask mask) {
            component_masks.push_back(mask);
            entity_masks.push_back({0});
        }

        // Function to clear the entity mask:
        void clear_entity_mask(unsigned int index) {
            for (int i = 0; i < MAX_ENTITIES; i++) {
                entity_masks[index].mask[i] = 0;
            }
        }

        // Function to clear a specific entity from all masks:
        void clear_entity(unsigned int entityID) {
            for (int i = 0; i < component_masks.size(); i++) {
                entity_masks[i].mask[entityID] = 0;
            }
        }

        // Function to test an Entity for eligibility in a System:
        void test_entity(ECSManager *manager, unsigned int entityID, quasarts_component_mask *entity_cmask) {
            // Loop through the Component masks:
            quasarts_component_mask *system_cmask;
            for (int i = 0; i < component_masks.size(); i++) {
                // Test an Entity against the Component mask at index:
                system_cmask = get_component_mask(i);

                if ((system_cmask->mask & entity_cmask->mask) == system_cmask->mask) {
                    // Masks match, this is a valid Entity for this System:
                    entity_masks[i].mask[entityID] = 1;
                } else {
                    // Masks don't match, this is not a valid Entity:
                    entity_masks[i].mask[entityID] = 0;
                }
            }
        }

        // Get the manager pointer:
        ECSManager *get_manager() {
            return manager;
        }

        // Set the manager pointer:
        void set_manager(ECSManager *manager_ptr) {
            manager = manager_ptr;
        }

        // Get the component mask:
        quasarts_component_mask *get_component_mask(unsigned int index) {
            return &(component_masks[index]);
        }

        // Get the entity mask:
        quasarts_entity_ID_mask *get_entity_ID_mask(unsigned int index) {
            return &(entity_masks[index]);
        }

        private:
        // The manager responsible for this System:
        ECSManager *manager;

        // Binary masks of component types used by the System:
        std::vector<quasarts_component_mask> component_masks;

        // Binary masks of entities that can be acted on by this System:
        std::vector<quasarts_entity_ID_mask> entity_masks;
    };
}