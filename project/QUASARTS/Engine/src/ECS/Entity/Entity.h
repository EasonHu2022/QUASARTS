#pragma once

// Library includes:
#include <string>

// Local includes:
#include "ECS/ECS-Common.h"

namespace Engine {
    class QS_API Entity {
        public:
        // Constructor and destructor:
        Entity(unsigned int ID);
        ~Entity();

        // Get entity ID:
        unsigned int get_entityID();

        // Get component mask:
        quasarts_component_mask get_componentMask();

        // Set component mask:
        void set_component_mask(quasarts_component_mask mask);
        
        // Add component type:
        void add_component_type(unsigned int component_type);

        // Remove component type:
        void remove_component_type(unsigned int component_type);

        // Get the entity name:
        std::string get_name() const;

        // Set the entity name:
        void set_name(std::string name);

        private:
        // Unique ID number for the Entity:
        unsigned int entityID;

        // Name of the Entity:
        std::string entityName;

        // Binary mask for component types:
        quasarts_component_mask componentMask;
    };
}