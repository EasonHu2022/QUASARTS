#include "ECS/Entity/Entity.h"

namespace Engine {
    // Constructor:
    Entity::Entity(unsigned int ID) {
        entityID = ID;
        componentMask = {0};
    }

    // Destructor:
    Entity::~Entity() {

    }

    // Get entity ID:
    unsigned int Entity::get_entityID() {
        return entityID;
    }

    // Get component mask:
    quasarts_component_mask Entity::get_componentMask() {
        return componentMask;
    }

    // Set component mask:
    void Entity::set_component_mask(quasarts_component_mask mask) {
        componentMask = mask;
    }

    // Add component type:
    void Entity::add_component_type(unsigned int component_type) {
        unsigned long long mask = 1 << component_type;
        if ((componentMask.mask & mask) != mask) {
            componentMask.mask += mask;
        }
    }

    // Remove component type:
    void Entity::remove_component_type(unsigned int component_type) {
        unsigned long long mask = 1 << component_type;
        if ((componentMask.mask & mask) == mask) {
            componentMask.mask -= mask;
        }
    }
}