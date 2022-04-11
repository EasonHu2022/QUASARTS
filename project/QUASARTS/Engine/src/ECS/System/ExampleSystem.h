#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "ECS/System/System.h"
#include "ECS/ECSManager.h"

namespace Engine {
    class QS_API ExampleSystem : public System {
        public:
        // Constructor and destructor:
        ExampleSystem() {
            // Set the component mask:
            quasarts_component_mask *mask = get_component_mask();
            mask->mask = 0;
            mask->mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        }
        ~ExampleSystem();

        // Update function:
        virtual void update() override {
            // Get the manager:
            ECSManager *active_manager = get_manager();

            // Get the entity ID mask:
            quasarts_entity_ID_mask *entities = get_entity_ID_mask();

            // Increment the transform position:
            TransformComponent transform;
            for (int i = 0; i < MAX_ENTITIES; i++) {
                if (entities->mask[i] == 1) {
                    // The entity is valid for the System:
                    transform = active_manager->get_component
                        <TransformComponent>(i, COMPONENT_TRANSFORM);

                    // Update the component and send it back:
                    transform.x += 0.1;
                    transform.y += 0.1;
                    transform.z += 0.1;

                    active_manager->replace_component<TransformComponent>
                        (i, COMPONENT_TRANSFORM, transform);
                }
            }
        }
    };
}