#pragma once

#include <list>

// Local includes:
#include "ECS/ECSManager.h"
#include "System.h"

namespace Engine {

#define MAX_ORBITS MAX_ENTITIES     // from ECS-Common.h

    class QS_API OrbitSystem : public System
    {
        /// <summary>
        /// List of orbiting entities in descending order of position dependence.
        /// For an entity E at index i in the list, all entities which orbit E will
        /// have indices greater than i.
        /// This is so that, in a given frame, a primary with its own orbit is never
        /// updated after its satellite(s), which would invalidate the relative 
        /// positions of its previously updated satellite(s).
        /// </summary>
        std::list<int> mOrderedOrbits;
        size_t mNumOrbits;
        int mEntityPrimaries[MAX_ORBITS]; // Track
        
    public:
        OrbitSystem();
        ~OrbitSystem();

        // Called when ENGINE starts.
        void init();
        // Called when GAME starts.
        int start();
        // Called for each GAME update.
        void update();
        // Called when GAME stops.
        int stop();
        void release();


        // Usage //
    public:
        /// <summary>
        /// Set the given entity's orbit primary, if it has a valid orbit component.
        /// The entity will orbit around the primary entity.
        /// </summary>
        /// <param name="aEntityId">ID of the orbiting entity.</param>
        /// <param name="aPrimaryEntityId">ID of the entity which the satellite will orbit around.</param>
        void set_orbit_primary(unsigned int const aEntityId, unsigned int const aPrimaryEntityId);

    };
}