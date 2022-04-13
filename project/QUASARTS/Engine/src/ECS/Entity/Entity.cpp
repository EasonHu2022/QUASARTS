#include "ECS/Entity/Entity.h"

namespace Engine {
    // Constructor:
    Entity::Entity(unsigned int ID) {
        entityID = ID;
        componentMask = {0};
        entityName = "Default";
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
        uint64_t mask = (uint64_t)1 << component_type;
        if ((componentMask.mask & mask) != mask) {
            componentMask.mask += mask;
        }
    }

    // Remove component type:
    void Entity::remove_component_type(unsigned int component_type) {
        uint64_t mask = (uint64_t)1 << component_type;
        if ((componentMask.mask & mask) == mask) {
            componentMask.mask -= mask;
        }
    }

    // Get the entity name:
    std::string Entity::get_name() const {
        return entityName;
    }

    // Set the entity name:
    void Entity::set_name(std::string name) {
        entityName = name;
    }
}