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
        /// Tree of orbiting entities.
        /// The orbits must be updated in descending order, from root to leaf.
        /// This is so that, in a given frame, a primary that has its own orbit is
        /// never updated after its satellite(s), which would invalidate the relative
        /// positions of any of its satellite(s) which were already updated in that
        /// frame.
        /// </summary>
    private:
        struct OrbitNode
        {
            unsigned int mEntityId;
            unsigned int mPrimaryId;
            std::shared_ptr<OrbitNode> pPrimaryNode;    // This node's Parent - if the primary entity lacks an orbit node, this points to the tree root.
            std::set<std::shared_ptr<OrbitNode>> mSatelliteNodes;    // This node's Children - list of all nodes which have this node as their primary.
            std::string tostring();
        };
        // Access point for the tree - does NOT represent an entity.
        std::shared_ptr<OrbitNode> mOrbitRoot;
        // Map of entity IDs to their corresponding orbit nodes.
        std::unordered_map<unsigned int, std::shared_ptr<OrbitNode>> mAllOrbitNodes;

        
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
        /// <returns>0 if successful, non-0 otherwise.</returns>
        int set_orbit_primary(unsigned int const aEntityId, unsigned int const aPrimaryEntityId);

        /// <summary>
        /// Set the periapse distance for the given entity's orbit component.
        /// </summary>
        /// <param name="aEntityId"></param>
        /// <param name="aDistPeriapse"></param>
        /// <returns></returns>
        int set_orbit_periapse(unsigned int const aEntityId, float const aDistPeriapse);

        /// <summary>
        /// Clears the entity's orbit component.
        /// Removes the entity's node from the orbit tree - the entity will no longer move due to orbital motion when the game is running.
        /// The entity's node is kept alive for re-use.
        /// </summary>
        /// <param name="aEntityId">ID of the entity whose orbit primary is to be cleared.</param>
        void clear_orbit(unsigned int const aEntityId);

        /// <summary>
        /// Destroy the entity's node.
        /// Calls clear_orbit() to safely remove the entity's node from the tree.
        /// </summary>
        /// <param name="aEntityId">ID of the entity whose orbit node will be destroyed.</param>
        void destroy_orbit(unsigned int const aEntityId);


        // Util //
    private:
        std::shared_ptr<OrbitSystem::OrbitNode> get_node(unsigned int const aEntityId);


        // Debug //
    private:
        std::string print_tree();
        void tree_tests();

    };

}