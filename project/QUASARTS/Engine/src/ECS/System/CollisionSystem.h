#pragma once

// Local includes:
#include "ECS/ECS-Common.h"
#include "ECS/System/System.h"
#include "ECS/ECSManager.h"

namespace Engine {
    class QS_API CollisionSystem : public System {

    public:
        // Constructor and destructor:
        CollisionSystem() {
            // Set the Component mask:
            quasarts_component_mask mask;
            mask.mask = 0;
            mask.mask += (uint64_t)1 << COMPONENT_COLLISION_SPHERE;

            // Add the Component mask to the System:
            add_component_mask(mask);
        }
        ~CollisionSystem();


        void update()
        {
            // Get the manager:
            ECSManager* active_manager = get_manager();

            // Get the entity ID mask:
            quasarts_entity_ID_mask* entities = get_entity_ID_mask(0);

            // Increment the transform position:
            CollisionSphereComponent sphere;
            for (int i = 0; i < MAX_ENTITIES; i++)
            {
                if (entities->mask[i] == 1)
                {
                    // The entity is valid for the System:
                    sphere = active_manager->get_component
                        <CollisionSphereComponent>(i, COMPONENT_COLLISION_SPHERE);

                    // Update the component and send it back:
                    

                    active_manager->replace_component<CollisionSphereComponent>
                        (i, COMPONENT_COLLISION_SPHERE, sphere);
                }
            }
        }

    };
}