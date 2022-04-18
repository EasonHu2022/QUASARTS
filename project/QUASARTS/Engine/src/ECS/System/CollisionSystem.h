#pragma once

// Local includes:
#include "ECS/ECSManager.h"

// Engine includes
#include "QuasartsEngine.h"


namespace Engine {

#define Q_MAX_COLLIDED MAX_ENTITIES // from PhysicsSystem

    class QS_API CollisionSystem : public System {


    public:
        void EV_CALLBACK_SIGNATURE(Collision); // Event handler forward declaration
        int mNumCollided; // Track how many components have had collisions in each frame.
        int mCollidedEntityIds[Q_MAX_COLLIDED]; // Track which components have collisions in each frame.
        std::vector<int> mCollided;
        

    public:
        // Constructor and destructor:
        CollisionSystem() {
            // Set the Component mask:
            quasarts_component_mask mask;
            mask.mask = 0;
            mask.mask += (uint64_t)1 << COMPONENT_COLLISION_SPHERE;
            add_component_mask(mask);

            // TODO - other collision components, e.g.:
            /*mask.mask = 0;
            mask.mask += (uint64_t)1 << COMPONENT_COLLISION_BOX;
            add_component_mask(mask);*/
            
            // Register Event handler for Collision events.
            EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(Collision));

            mNumCollided = 0;
            for (int i = 0; i < Q_MAX_COLLIDED; ++i)
            {
                mCollidedEntityIds[i] = -1;
            }
        }
        ~CollisionSystem();


        void update()
        {
            // Get the manager:
            ECSManager* active_manager = get_manager();

            // Get the entity ID mask:
            quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(0);

            // Increment the transform position:
            CollisionSphereComponent sphere;
            for (int i = 0; i < MAX_ENTITIES; i++)
            {
                if (entitiesSpheres->mask[i] == 1)
                {
                    // The entity is valid for the System:
                    sphere = active_manager->get_component
                        <CollisionSphereComponent>(i, COMPONENT_COLLISION_SPHERE);

                    // Update the component and send it back:


                    active_manager->replace_component<CollisionSphereComponent>
                        (i, COMPONENT_COLLISION_SPHERE, sphere);
                }
            }


            // TODO : handle collisions

            // Reset collision tracking before next physics update.
            for (int i = 0; i < mNumCollided; ++i)
            {
                unsigned int entityId = mCollidedEntityIds[i];
                CollisionSphereComponent componentCopy = get_manager()->get_component<CollisionSphereComponent>(entityId, (unsigned)COMPONENT_COLLISION_SPHERE);
                componentCopy.clear_overlaps();
                get_manager()->replace_component(entityId, (unsigned)COMPONENT_COLLISION_SPHERE, componentCopy);
                mCollidedEntityIds[i] = -1;
            }
            mNumCollided = 0;
        }

        void EV_CALLBACK_SIGNATURE(Collision)
        {
            int entityId0 = -1, componentType0 = -1;
            int entityId1 = -1, componentType1 = -1;
            if (!evt.find_argument(&entityId0, "entity0") || !evt.find_argument(&componentType0, "componentType0") ||
                !evt.find_argument(&entityId1, "entity1") || !evt.find_argument(&componentType1, "componentType1"))
            {
                QERROR("CollisionSystem received a Collision event with missing arguments: {0}", evt.to_string().c_str());
                return;
            }

            // Check the arguments are valid.
            if (!get_manager()->has_component((unsigned)entityId0, (unsigned)COMPONENT_COLLISION_SPHERE) ||
                !get_manager()->has_component((unsigned)entityId1, (unsigned)COMPONENT_COLLISION_SPHERE))
            {
                char msg[128];
                snprintf(msg, 128, "CollisionSystem received a Collision event with entity IDs of entities which do not have collision sphere components: %s", evt.to_string().c_str());
                QERROR(msg);
                return;
            }

            // Replace each component with new data to provide information about the collision.
            CollisionSphereComponent componentCopy;

            // component 0
            componentCopy = get_manager()->get_component<CollisionSphereComponent>((unsigned)entityId0, (unsigned)COMPONENT_COLLISION_SPHERE);
            componentCopy.add_overlap(entityId1, componentType1);
            get_manager()->replace_component((unsigned)entityId0, (unsigned)COMPONENT_COLLISION_SPHERE, componentCopy);

            if (componentCopy.mNumOverlaps == 1) // did not have overlaps before this one, was not yet tracked
            {
                mCollidedEntityIds[mNumCollided] = entityId0;
                ++mNumCollided;
            }


            // component 1
            componentCopy = get_manager()->get_component<CollisionSphereComponent>((unsigned)entityId1, (unsigned)COMPONENT_COLLISION_SPHERE);
            componentCopy.add_overlap(entityId0, componentType0);
            get_manager()->replace_component((unsigned)entityId1, (unsigned)COMPONENT_COLLISION_SPHERE, componentCopy);


            // Track which components have had collisions this frame.
            bool alreadyCollided0 = false, alreadyCollided1 = false;
            for (int i = 0; i < mNumCollided; ++i)
            {
                if (mCollidedEntityIds[i] == entityId0) alreadyCollided0 = true;
                if (mCollidedEntityIds[i] == entityId1) alreadyCollided1 = true;
            }
            if (!alreadyCollided0)
            {
                mCollidedEntityIds[mNumCollided] = entityId0;
                ++mNumCollided;
            }
            if (!alreadyCollided1)
            {
                mCollidedEntityIds[mNumCollided] = entityId1;
                ++mNumCollided;
            }
        }

    };
}