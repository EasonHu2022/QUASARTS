#pragma once

// Local includes:
#include "ECS/ECSManager.h"
#include "System.h"

#include "Event/EventModule.h"
#include "Scene/PhysicsSystem.h"


namespace Engine {

#define Q_MAX_COLLIDED MAX_ENTITIES // from PhysicsSystem

    class QS_API CollisionSystem : public System
    {

    public:
        int mNumCollided; // Track how many components have had collisions in each frame.
        int mCollidedEntityIds[Q_MAX_COLLIDED]; // Track which components have collisions in each frame.
        std::vector<int> mCollided;
        

    public:
        // Constructor and destructor:
        CollisionSystem();
        ~CollisionSystem();

        void init();
        int start();
        void update();
        int stop();
        void release();


        // Usage //
    public:
        /// <summary>
        /// Pair a collision component with a collision object in the physics system.
        /// </summary>
        /// <param name="entityId"></param>
        /// <param name="aComponentType"></param>
        void init_collision_component(unsigned int const aEntityId, int const aComponentType);

        /// <summary>
        /// Unpair a collision component from its collision object in the physics system.
        /// </summary>
        /// <param name="entityId"></param>
        /// <param name="aComponentType"></param>
        void release_collision_component(unsigned int const aEntityId, int const aComponentType);

        /// <summary>
        /// Change the collision object's offset (position relative to its parent entity).
        /// The collision object will follow its parent entity with this offset.
        /// </summary>
        /// <param name="aEntityId">ID of the collision component's parent entity.</param>
        /// <param name="aDeltaOffset">The vector which will be added to the current offset.</param>
        void move_collision_component(unsigned int const aEntityId, int const aComponentType, glm::vec3 const aDeltaOffset);


        // Util //
    private:
        unsigned int get_mask_index(int const aComponentType);

        // Events
    public:
        void EV_CALLBACK_SIGNATURE(Collision);

    };
}