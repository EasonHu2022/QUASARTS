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


    void CollisionSystem::init()
    {
        //component_tests_init();

    } // init()


    int CollisionSystem::start()
    {
        return 0;

    } // start()


    void CollisionSystem::update()
    {
        //component_tests_running();

        ECSManager* active_manager = get_manager();
        CollisionSphereComponent* collisionSphere;

        // Perform late-update collision handling.
        for (int i = 0; i < mNumCollided; ++i)
        {
            unsigned int entityId = mCollidedEntityIds[i];
            collisionSphere = active_manager->get_component<CollisionSphereComponent>(entityId, (unsigned)COMPONENT_COLLISION_SPHERE);

            for (int j = 0; j < collisionSphere->mNumOverlaps; ++j)
            {
                // TODO : late-update collision handling
            }

            // Reset collision tracking for next frame.
            collisionSphere->mNumOverlaps = 0;
        }
        mNumCollided = 0;


        // Update all collision object positions for next physics update.
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(COMPONENT_COLLISION_SPHERE));
        TransformComponent* transf;
        unsigned int collisionType = COMPONENT_COLLISION_SPHERE;
        unsigned int transformType = COMPONENT_TRANSFORM;
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (entitiesSpheres->mask[i] == 1)
            {
                // The entity is valid for the System:
                collisionSphere = active_manager->get_component
                    <CollisionSphereComponent>(i, collisionType);

                transf = active_manager->get_component
                    <TransformComponent>(i, transformType);


                // Update the component.
                PhysicsSystem::Instance()->set_collision_object_position(collisionSphere->mCollisionObjectId, collisionSphere->mLocalOffset + transf->position);
            }
        }

    } // update()


    int CollisionSystem::stop()
    {
        return 0;

    } // stop()


    void CollisionSystem::release()
    {

    } // release()

    // Set up all the data required for the component to function:
	void CollisionSystem::initialize_components() {
		// Get the manager and entity mask:
		ECSManager* active_manager = get_manager();
		quasarts_entity_ID_mask *entitiesSpheres = get_entity_ID_mask(get_mask_index(COMPONENT_COLLISION_SPHERE));
		CollisionSphereComponent *collisionSphere;

		// Loop through entities:
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (entitiesSpheres->mask[i] == 1) // Entity [i] with orbit component.
			{
				init_collision_component(i, COMPONENT_COLLISION_SPHERE);
			}
		}
		/* Alternative method (slower, but you can ignore the mask if you need):
		std::vector<unsigned int> entities = active_manager->get_entity_ID_match();
		for (int i = 0; i < entities.size(); i++) {
			if (active_manager->has_component(entities[i], COMPONENT_ORBIT) == true) {
				init_collision_component(entities[i], COMPONENT_COLLISION_SPHERE);
			}
		}
		*/
    }

    // Usage //

    void CollisionSystem::init_collision_component(unsigned int const aEntityId, int const aComponentType)
    {
        ECSManager* active_manager = get_manager();

        // Get entity if it has the correct component.
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(aComponentType));
        if (entitiesSpheres->mask[aEntityId] == 1)
        {
            // Get component.
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

            collisionSphere->mCollisionObjectId = PhysicsSystem::Instance()->assign_collision_sphere(aEntityId, (transf->position + collisionSphere->mLocalOffset), collisionSphere->mRadius);
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
        if (aComponentType != COMPONENT_COLLISION_SPHERE) return;
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(aComponentType));
        if (entitiesSpheres->mask[aEntityId] == 1)
        {
            // Get component and update.
            ECSManager* active_manager = get_manager();
            CollisionSphereComponent* collisionSphere = active_manager->get_component
                <CollisionSphereComponent>(aEntityId, COMPONENT_COLLISION_SPHERE);

            collisionSphere->mLocalOffset += aDeltaOffset;

            // Update object in physics system.
            TransformComponent* transform = active_manager->get_component
                <TransformComponent>(aEntityId, COMPONENT_TRANSFORM);
            PhysicsSystem::Instance()->set_collision_object_position(collisionSphere->mCollisionObjectId, transform->position + collisionSphere->mLocalOffset);
        }
    } // move_collision_component()


    void CollisionSystem::set_collision_sphere_radius(unsigned int const aEntityId, float const aNewRadius)
    {
        // Get entity if it has the correct component.
        quasarts_entity_ID_mask* entitiesSpheres = get_entity_ID_mask(get_mask_index(COMPONENT_COLLISION_SPHERE));
        if (entitiesSpheres->mask[aEntityId] == 1)
        {
            // Get component and update.
            ECSManager* active_manager = get_manager();
            CollisionSphereComponent* collisionSphere = active_manager->get_component
                <CollisionSphereComponent>(aEntityId, COMPONENT_COLLISION_SPHERE);

            collisionSphere->mRadius = aNewRadius;

            // Update object in physics system.
            PhysicsSystem::Instance()->set_collision_sphere_radius(collisionSphere->mCollisionObjectId, aNewRadius);
        }
    } // set_collision_sphere_radius()



    // Events //

    void CollisionSystem::EV_CALLBACK_SIGNATURE(Collision)
    {
        int entityId0 = -1, componentType0 = -1;
        int entityId1 = -1, componentType1 = -1;
        if (!(evt.find_argument(&entityId0, "entity0") && evt.find_argument(&componentType0, "componentType0") &&
              evt.find_argument(&entityId1, "entity1") && evt.find_argument(&componentType1, "componentType1")))
        {
            QERROR("CollisionSystem received a Collision event with missing arguments: {0}", evt.to_string());
            return;
        }

        // Check the arguments are valid.
        if (!get_manager()->has_component((unsigned)entityId0, (unsigned)COMPONENT_COLLISION_SPHERE) ||
            !get_manager()->has_component((unsigned)entityId1, (unsigned)COMPONENT_COLLISION_SPHERE))
        {
            QERROR("CollisionSystem received a Collision event with entity IDs of entities which do not have collision sphere components: {0}", evt.to_string());
            return;
        }

        // Update both components with information about the collision.
        CollisionSphereComponent* sphere;


        // component 0
        sphere = get_manager()->get_component<CollisionSphereComponent>((unsigned)entityId0, (unsigned)COMPONENT_COLLISION_SPHERE);

        // Don't repeat components in the system's collision tracker:
        // If this component has not already had a collision, add it to the tracker for this frame.
        if (sphere->mNumOverlaps == 0)
        {
            mCollidedEntityIds[mNumCollided] = entityId0;
            ++mNumCollided;
        }
        sphere->add_overlap(entityId1, componentType1);


        // component 1
        sphere = get_manager()->get_component<CollisionSphereComponent>((unsigned)entityId1, (unsigned)COMPONENT_COLLISION_SPHERE);

        // Don't repeat components in the system's collision tracker:
        // If this component has not already had a collision, add it to the tracker for this frame.
        if (sphere->mNumOverlaps == 0)
        {
            mCollidedEntityIds[mNumCollided] = entityId1;
            ++mNumCollided;
        }
        sphere->add_overlap(entityId0, componentType0);

    } // EV_CALLBACK_SIGNATURE(Collision)


    // Util //

    unsigned int CollisionSystem::get_mask_index(int const aComponentType)
    {
        switch (aComponentType)
        {
        case COMPONENT_COLLISION_SPHERE: return 0;
        }
        return -1;
    }


    // Debug //

    void CollisionSystem::component_tests_init()
    {
        ECSManager* active_manager = get_manager();

        // Create entity 0 with paired collision component.
        entity0Id = active_manager->create_entity();
        Entity* entity0 = active_manager->get_entity(entity0Id);
        if (entity0 == nullptr) {
            QDEBUG("CollisionSystem: component_tests(): Failed to create/retrieve entity {0}", entity0Id); return;
        }
        QDEBUG("CollisionSystem: component_tests(): Created and retrieved entity {0}", entity0Id);

        active_manager->create_component<CollisionSphereComponent>(entity0Id, COMPONENT_COLLISION_SPHERE);
        CollisionSphereComponent* sphere0 = active_manager->get_component<CollisionSphereComponent>(entity0Id, COMPONENT_COLLISION_SPHERE);
        if (sphere0 == nullptr) {
            QDEBUG("CollisionSystem: component_tests(): Failed to create/retrieve orbit", entity0Id); return;
        }
        QDEBUG("CollisionSystem: component_tests(): Created and retrieved orbit", entity0Id);

        init_collision_component(entity0Id, COMPONENT_COLLISION_SPHERE);
        QDEBUG("New object: {0}", PhysicsSystem::Instance()->object_tostring(sphere0->mCollisionObjectId));


        // Create entity 1 with paired collision component.
        entity1Id = active_manager->create_entity();
        Entity* entity1 = active_manager->get_entity(entity1Id);
        if (entity1 == nullptr) {
            QDEBUG("CollisionSystem: component_tests(): Failed to create/retrieve entity {0}", entity1Id); return;
        }
        QDEBUG("CollisionSystem: component_tests(): Created and retrieved entity {0}", entity1Id);

        active_manager->create_component<CollisionSphereComponent>(entity1Id, COMPONENT_COLLISION_SPHERE);
        CollisionSphereComponent* sphere1 = active_manager->get_component<CollisionSphereComponent>(entity1Id, COMPONENT_COLLISION_SPHERE);
        if (sphere1 == nullptr) {
            QDEBUG("CollisionSystem: component_tests(): Failed to create/retrieve orbit", entity1Id); return;
        }
        QDEBUG("CollisionSystem: component_tests(): Created and retrieved orbit", entity1Id);

        init_collision_component(entity1Id, COMPONENT_COLLISION_SPHERE);
        QDEBUG("New object: {0}", PhysicsSystem::Instance()->object_tostring(sphere1->mCollisionObjectId));


        // Release sphere 0.
        QDEBUG("Releasing sphere 0...");
        {
            int objId = sphere0->mCollisionObjectId; // dangerous variable to keep around: assign in short-term scope
            release_collision_component(entity0Id, COMPONENT_COLLISION_SPHERE);
            QDEBUG("Object after release: {0}", PhysicsSystem::Instance()->object_tostring(objId));
        }


        // Re-assign sphere 0.
        QDEBUG("Re-assigning sphere 0...");
        init_collision_component(entity0Id, COMPONENT_COLLISION_SPHERE);
        QDEBUG("New assigned object: {0}", PhysicsSystem::Instance()->object_tostring(sphere0->mCollisionObjectId));

        
        // Changing component offsets.
        QDEBUG("Moving components...");
        glm::vec3 move0(5,0,0);
        move_collision_component(entity0Id, COMPONENT_COLLISION_SPHERE, move0);
        QDEBUG("- Object 0 after move: {0}", PhysicsSystem::Instance()->object_tostring(sphere0->mCollisionObjectId));

        glm::vec3 move1(0, -3, 0);
        move_collision_component(entity1Id, COMPONENT_COLLISION_SPHERE, move1);
        QDEBUG("- Object 1 after move: {0}", PhysicsSystem::Instance()->object_tostring(sphere1->mCollisionObjectId));


        // Set up a collision.
        QDEBUG("Setting up collision...");
        move_collision_component(entity1Id, COMPONENT_COLLISION_SPHERE, move0);
        QDEBUG("- Object 1 after move: {0}", PhysicsSystem::Instance()->object_tostring(sphere1->mCollisionObjectId));
        oscillationTimer = 6;
        movement_per_second = glm::vec3(0, 1, 0);
        oneshot = true;


        // Set new sphere radii.
        QDEBUG("Setting new sphere radii...");
        set_collision_sphere_radius(entity0Id, 1.5);
        set_collision_sphere_radius(entity1Id, 0.3);
        QDEBUG("- Object 0 after resize: {0}", PhysicsSystem::Instance()->object_tostring(sphere0->mCollisionObjectId));
        QDEBUG("- Object 1 after resize: {0}", PhysicsSystem::Instance()->object_tostring(sphere1->mCollisionObjectId));

    }


    void CollisionSystem::component_tests_running()
    {
        QTime dT = TimeModule::Instance()->get_frame_delta_time();

        ECSManager* active_manager = get_manager();
        CollisionSphereComponent* sphere0 = active_manager->get_component<CollisionSphereComponent>(entity0Id, COMPONENT_COLLISION_SPHERE);
        CollisionSphereComponent* sphere1 = active_manager->get_component<CollisionSphereComponent>(entity1Id, COMPONENT_COLLISION_SPHERE);

        if (oneshot)
        {
            oneshot = false;
            QDEBUG("component_tests_running() called:");
            //QDEBUG("- Object 0: {0}", PhysicsSystem::Instance()->object_tostring(sphere0->mCollisionObjectId));
        }

        if (sphere0->mNumOverlaps > 0)
        {
            QDEBUG("Sphere 0 overlaps!! {0}", sphere0->overlap_tostring());
        }
        if (sphere1->mNumOverlaps > 0)
        {
            QDEBUG("Sphere 1 overlaps!! {0}", sphere0->overlap_tostring());
        }

        if (oscillationTimer < 0)
        {
            oscillationTimer += 6; // change direction every 6 seconds
            movement_per_second *= -1;
        }
        oscillationTimer -= dT;
        TransformComponent* transform1 = active_manager->get_component<TransformComponent>(entity1Id, COMPONENT_TRANSFORM);
        transform1->position += (movement_per_second * (float)dT.sec());

        if (updateTimer < 0)
        {
            updateTimer += 0.5;
            QDEBUG("- Object 1: {0}", PhysicsSystem::Instance()->object_tostring(sphere1->mCollisionObjectId));
        }
        updateTimer -= dT;

    } // component_tests_running()
}