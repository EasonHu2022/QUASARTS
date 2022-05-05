#include "CollisionSystem.h"


namespace Engine
{
    CollisionSystem::CollisionSystem() {
        // Set the Component mask:
        quasarts_component_mask mask;
        mask.mask = 0;
        mask.mask += (uint64_t)1 << COMPONENT_COLLISION_SPHERE;
        add_component_mask(mask);

        // TODO - other collision components, e.g. BOX, CAPSULE, &c
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

    } // CollisionSystem()


    void CollisionSystem::update()
    {
        CollisionSphereComponent* collisionSphere, otherSphere;

        // Handle collisions from last physics update.
        for (int i = 0; i < mNumCollided; ++i)
        {
            unsigned int entityId = mCollidedEntityIds[i];
            collisionSphere = get_manager()->get_component<CollisionSphereComponent>(entityId, (unsigned)COMPONENT_COLLISION_SPHERE);

            for (int j = 0; j < collisionSphere->mNumOverlaps; ++j)
            {
                // TODO : collision handling
            }

            // Reset collision tracking for next frame.
            collisionSphere->mNumOverlaps = 0;
        }
        mNumCollided = 0;


        // Update all collision object positions for next physics update.
        ECSManager* active_manager = get_manager();
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(COMPONENT_COLLISION_SPHERE));
        TransformComponent* transf;
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (entitiesSpheres->mask[i] == 1)
            {
                // The entity is valid for the System:
                collisionSphere = active_manager->get_component
                    <CollisionSphereComponent>(i, COMPONENT_COLLISION_SPHERE);

                transf = active_manager->get_component
                    <TransformComponent>(i, COMPONENT_TRANSFORM);


                // Update the component.
                PhysicsSystem::Instance()->set_collision_object_position(collisionSphere->mCollisionObjectId, collisionSphere->mLocalOffset + transf->position);
            }
        }
    }


    // Usage //

    void CollisionSystem::init_collision_component(unsigned int const aEntityId, int const aComponentType)
    {
        // Get entity if it has the correct component.
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(aComponentType));
        if (entitiesSpheres->mask[aEntityId] == 1)
        {
            // Get component.
            ECSManager* active_manager = get_manager();
            CollisionSphereComponent* collisionSphere = active_manager->get_component
                <CollisionSphereComponent>(aEntityId, COMPONENT_COLLISION_SPHERE);

            // Check collision component is not already initialised.
            if (collisionSphere->mCollisionObjectId != -1)
            {
                QDEBUG("Cannot initialise collision component: entity {0} already has a collision sphere component.", aEntityId);
                return;
            }

            // Initialise component.
            TransformComponent* transf = active_manager->get_component
                <TransformComponent>(aEntityId, COMPONENT_TRANSFORM);

            collisionSphere->mCollisionObjectId = PhysicsSystem::Instance()->assign_collision_sphere(aEntityId, transf->position, Q_DEFAULT_SPHERE_RADIUS);
        }
    } // init_collision_component()


    void CollisionSystem::release_collision_component(unsigned int const aEntityId, int const aComponentType)
    {
        // Get entity if it has the correct component.
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(aComponentType));
        if (entitiesSpheres->mask[aEntityId] == 1)
        {
            // Get component.
            ECSManager* active_manager = get_manager();
            CollisionSphereComponent* collisionSphere = active_manager->get_component
                <CollisionSphereComponent>(aEntityId, COMPONENT_COLLISION_SPHERE);

            // Check if collision component is not initialised.
            if (collisionSphere->mCollisionObjectId == -1)
            {
                QDEBUG("Cannot release collision component: entity {0} collision sphere component is not initialised.", aEntityId);
                return;
            }

            // Unassign the collision object in the physics system.
            PhysicsSystem::Instance()->unassign_collision_object(collisionSphere->mCollisionObjectId);
            collisionSphere->mCollisionObjectId = -1;
        }
    } // remove_collision_component()


    void CollisionSystem::move_collision_component(unsigned int const aEntityId, int const aComponentType, glm::vec3 const aDeltaOffset)
    {
        // Get entity if it has the correct component.
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(aComponentType));
        if (entitiesSpheres->mask[aEntityId] == 1)
        {
            // Get component.
            ECSManager* active_manager = get_manager();
            CollisionSphereComponent* collisionSphere = active_manager->get_component
                <CollisionSphereComponent>(aEntityId, COMPONENT_COLLISION_SPHERE);

            collisionSphere->mLocalOffset += aDeltaOffset;
        }
    } // move_collision_component()



    // Events //

    void CollisionSystem::EV_CALLBACK_SIGNATURE(Collision)
    {
        int entityId0 = -1, componentType0 = -1;
        int entityId1 = -1, componentType1 = -1;
        if (!(evt.find_argument(&entityId0, "entity0") && evt.find_argument(&componentType0, "componentType0") &&
              evt.find_argument(&entityId1, "entity1") && evt.find_argument(&componentType1, "componentType1")))
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

        // Update both components with information about the collision.
        CollisionSphereComponent* componentPtr;

        // component 0
        componentPtr = get_manager()->get_component<CollisionSphereComponent>((unsigned)entityId0, (unsigned)COMPONENT_COLLISION_SPHERE);
        // Don't repeat components in the system's tracking array.
        if (componentPtr->mNumOverlaps == 0)
        {
            mCollidedEntityIds[mNumCollided] = entityId0;
            ++mNumCollided;
        }
        componentPtr->add_overlap(entityId1, componentType1);

        // component 1
        componentPtr = get_manager()->get_component<CollisionSphereComponent>((unsigned)entityId1, (unsigned)COMPONENT_COLLISION_SPHERE);
        // Don't repeat components in the system's tracking array.
        if (componentPtr->mNumOverlaps == 0)
        {
            mCollidedEntityIds[mNumCollided] = entityId1;
            ++mNumCollided;
        }
        componentPtr->add_overlap(entityId0, componentType0);

    } // CollisionSystem()


    // Util //

    unsigned int CollisionSystem::get_mask_index(int const aComponentType)
    {
        switch (aComponentType)
        {
        case COMPONENT_COLLISION_SPHERE: return 0;
        }
        return -1;
    }
}