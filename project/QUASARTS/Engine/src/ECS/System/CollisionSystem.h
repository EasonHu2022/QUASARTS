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
        // singleton
    private:
        static CollisionSystem* instance;
        CollisionSystem();
    public:
        static CollisionSystem* Instance();
        ~CollisionSystem();

    public:
        int mNumCollided; // Track how many components have had collisions in each frame.
        int mCollidedEntityIds[Q_MAX_COLLIDED]; // Track which components have collisions in each frame.
        

    public:
        void init();
        int start();
        void update();
        int stop();
        void release();
        // Set up all the data required for the component to function:
		void initialize_components() override;

        void reset();


        // Usage //
    public:
        /// <summary>
        /// Pair a collision component with a collision object in the physics system.
        /// </summary>
        /// <param name="entityId"></param>
        /// <param name="aComponentType">Defined value, e.g., COMPONENT_COLLISION_SPHERE</param>
        void init_collision_component(unsigned int const aEntityId, int const aComponentType);

        /// <summary>
        /// Unpair a collision component from its collision object in the physics system.
        /// </summary>
        /// <param name="entityId"></param>
        /// <param name="aComponentType">Defined value, e.g., COMPONENT_COLLISION_SPHERE</param>
        void release_collision_component(unsigned int const aEntityId, int const aComponentType);

        /// <summary>
        /// Change the collision object's offset (position relative to its parent entity).
        /// The collision object will follow its parent entity with this offset.
        /// </summary>
        /// <param name="aEntityId"></param>
        /// <param name="aDeltaOffset">The vector which will be added to the current offset.</param>
        void move_collision_component(unsigned int const aEntityId, int const aComponentType, glm::vec3 const aDeltaOffset);

        /// <summary>
        /// Sets radius of the collision sphere component and its paired collision object, if the entity has a valid collision sphere component.
        /// </summary>
        /// <param name="aEntityId"></param>
        /// <param name="aNewRadius"></param>
        void set_collision_sphere_radius(unsigned int const aEntityId, float const aNewRadius);


        // Util //
    private:
        unsigned int get_mask_index(int const aComponentType);


        // Events //
    public:
        void EV_CALLBACK_SIGNATURE(Collision);


        // Debug //
    private:
        void component_tests_init();
        void component_tests_running();
        int entity0Id, entity1Id;
        QTime oscillationTimer, updateTimer;
        glm::vec3 movement_per_second;
        bool oneshot;
        bool oneshot2;
        void script_test();

    };
}