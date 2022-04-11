#pragma once

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

        private:
        // Unique ID number for the entity:
        unsigned int entityID;

        // Binary mask for component types:
        quasarts_component_mask componentMask;
    };
}