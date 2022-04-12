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
        System() : manager(nullptr), component_mask({0}), entity_mask({0}) {}
        ~System() {}

        // Function to clear the component mask:
        void clear_component_mask() {
            component_mask.mask = 0;
        }

        // Function to clear the entity mask:
        void clear_entity_mask() {
            for (int i = 0; i < MAX_ENTITIES; i++) {
                entity_mask.mask[i] = 0;
            }
        }

        // Function to clear a specific entity from mask:
        void clear_entity(unsigned int entityID) {
            entity_mask.mask[entityID] = 0;
        }

        // Function to add component type to System:
        void add_component_type(unsigned int component_type) {
            uint64_t mask = (uint64_t)1 << component_type;
            if ((component_mask.mask & mask) != mask) {
                component_mask.mask += mask;
            }
        }

        // Function to remove component type from System:
        void remove_component_type(unsigned int component_type) {
            uint64_t mask = (uint64_t)1 << component_type;
            if ((component_mask.mask & mask) == mask) {
                component_mask.mask -= mask;
            }
        }

        // Function to test an entity for eligibility in a System:
        void test_entity(quasarts_component_mask mask, unsigned int entityID) {
            if ((mask.mask & component_mask.mask) == component_mask.mask) {
                // Masks match, this is a valid entity for this System:
                entity_mask.mask[entityID] = 1;
            } else {
                entity_mask.mask[entityID] = 0;
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
        quasarts_component_mask *get_component_mask() {
            return &component_mask;
        }

        // Get the entity mask:
        quasarts_entity_ID_mask *get_entity_ID_mask() {
            return &entity_mask;
        }

        private:
        // The manager responsible for this System:
        ECSManager *manager;

        // Binary mask of component types used by the System:
        quasarts_component_mask component_mask;

        // Binary mask of entities that can be acted on by this System:
        quasarts_entity_ID_mask entity_mask;
    };
}