#pragma once

// Library includes:
#include <vector>

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    class QS_API ECSManager;
    class QS_API System {
        public:
        // Constructor and destructor:
        System();
        ~System();

        // Update function (empty - to be redefined in specific Systems):
        virtual void update();

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