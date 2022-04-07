#pragma once

// Library includes:
#include <vector>

// Local includes:
#include "Core/Core.h"
#include "ECS/ECS-Common.h"
#include "ECS/Entity/Entity.h"
#include "ECS/ECSManager.h"

namespace Engine {
    class QS_API System {
        public:
        // Constructor and destructor:
        System();
        ~System();

        // Update function (empty - to be redefined in specific Systems):
        void update(ECSManager *manager);

        // Function to clear the component mask:
        void clear_component_mask();

        // Function to clear the entity mask:
        void clear_entity_mask();

        // Function to clear a specific entity from mask:
        void clear_entity(unsigned int entityID);

        // Function to add component type to System:
        void add_component_type(unsigned int component_type);

        // Function to remove component type from System:
        void remove_component_type(unsigned int component_type);

        // Function to test an entity for eligibility in a System:
        void test_entity(quasarts_component_mask mask, unsigned int entityID);

        // Function to refresh the System (reload valid entities):
        void refresh(std::vector<Entity> &entities);

        private:
        // Binary mask of component types used by the System:
        quasarts_component_mask component_mask;

        // Binary mask of entities that can be acted on by this System:
        quasarts_entity_ID_mask entity_mask;
    };
}