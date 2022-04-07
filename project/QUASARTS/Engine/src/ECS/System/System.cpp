#include "ECS/System/System.h"

namespace Engine {
    // Constructor:
    System::System() {

    }

    // Destructor:
    System::~System() {

    }

    // Update function (empty - to be redefined in specific Systems):
    void System::update(ECSManager *manager) {

    }

    // Function to clear the component mask:
    void System::clear_component_mask() {
        component_mask.mask = 0;
    }

    // Function to clear the entity mask:
    void System::clear_entity_mask() {
        for (int i = 0; i < MAX_ENTITIES; i++) {
            entity_mask.mask[i] = 0;
        }
    }

    // Function to clear a specific entity from mask:
    void System::clear_entity(unsigned int entityID) {
        entity_mask.mask[entityID] = 0;
    }

    // Function to add component type to System:
    void System::add_component_type(unsigned int component_type) {
        uint64_t mask = (uint64_t)1 << component_type;
        if ((component_mask.mask & mask) != mask) {
            component_mask.mask += mask;
        }
    }

    // Function to remove component type from System:
    void System::remove_component_type(unsigned int component_type) {
        uint64_t mask = (uint64_t)1 << component_type;
        if ((component_mask.mask & mask) == mask) {
            component_mask.mask -= mask;
        }
    }

    // Function to test an entity for eligibility in a System:
    void System::test_entity(quasarts_component_mask mask, unsigned int entityID) {
        if ((mask.mask & component_mask.mask) == component_mask.mask) {
            // Masks match, this is a valid entity for this System:
            entity_mask.mask[entityID] = 1;
        } else {
            entity_mask.mask[entityID] = 0;
        }
    }

    // Function to refresh the System (reload valid entities):
    void System::refresh(std::vector<Entity> &entities) {
        // First clear the entity mask:
        clear_entity_mask();

        // Loop through the entities, checking for bit mask matches:
        for (int i = 0; i < entities.size(); i++) {
            test_entity(entities[i].get_componentMask(), entities[i].get_entityID());
        }
    }
}