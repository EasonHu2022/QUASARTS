#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

// Engine includes
#include "glm/vec3.hpp"

#include  <sstream>


namespace Engine {

// Maximum number of overlaps recorded per collision component per frame.
#define Q_MAX_OVERLAPS  4

    struct QS_API CollisionSphereComponent
    {
        CollisionSphereComponent() :
            mLocalOffset(0,0,0),
            mRadius(1.f),
            mCollisionObjectId(-1),
            mNumOverlaps(0)
        { }

        // Local offset
        glm::vec3 mLocalOffset;
        // Collision sphere radius (centred on local offset)
        float mRadius;
        // ID of corresponding collision object in the collision world.
        int mCollisionObjectId;
        // Number of overlaps in this frame.
        int mNumOverlaps;
        // The ID of overlapping collision components' parent entities for this frame.
        int mOverlapEntityIds[Q_MAX_OVERLAPS];
        // Overlapping collision components' types in this frame.
        int mOverlapComponentTypes[Q_MAX_OVERLAPS];

        /// <summary>
        /// Adds information about an overlap with another collision component IF this component does not already have maximum number of overlaps in this frame.
        /// </summary>
        /// <param name="aOtherEntityId">ID of other collision component's parent entity.</param>
        /// <param name="aOtherComponentType">Other collision component's type.</param>
        void add_overlap(const int aOtherEntityId, const int aOtherComponentType)
        {
            if (mNumOverlaps == Q_MAX_OVERLAPS)
            {
                /*QERROR("add_overlap() called on CollisionSphereComponent (with collision object ID {0}) with maximum overlaps;"
                    "collision with entity {1} could not be added in this frame.", mCollisionObjectId, mOverlapEntityId);*/
                return;
            }
            mOverlapEntityIds[mNumOverlaps] = aOtherEntityId;
            mOverlapComponentTypes[mNumOverlaps] = aOtherComponentType;
            ++mNumOverlaps;
        }


        std::string overlap_tostring()
        {
            std::ostringstream ostr;
            ostr << "Overlaps: " << mNumOverlaps << " ( ";
            for (int i = 0; i < mNumOverlaps; ++i)
            {
                ostr << mOverlapEntityIds[i] << " ";
            }
            ostr << ")";
            return ostr.str();
        }
    };



    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, CollisionSphereComponent &collisionSphere) {
        inStream
            >> collisionSphere.mLocalOffset.x >> collisionSphere.mLocalOffset.y >> collisionSphere.mLocalOffset.z
            >> collisionSphere.mRadius;

        // TODO : create collision object in PhysicsSystem to populate mCollisionObjectId

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const CollisionSphereComponent &collisionSphere) {
        outStream
        << collisionSphere.mLocalOffset.x << " " << collisionSphere.mLocalOffset.y << " " << collisionSphere.mLocalOffset.z
            << " " << collisionSphere.mRadius;
        
        return outStream;
    }
}