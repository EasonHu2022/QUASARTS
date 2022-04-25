#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

// Engine includes
#include "glm/vec3.hpp"


namespace Engine {

#define Q_MAX_OVERLAPS  4

    struct QS_API CollisionSphereComponent {
        // Local offset
        glm::vec3 mLocalOffset;
        // Collision sphere radius (centred on local offset)
        float mRadius;
        // ID of corresponding collision object in the collision world.
        int mCollisionObjectId;
        // Number of overlaps in this frame.
        int mNumOverlaps;
        // The ID of overlapping collision components' parent entities for this frame.
        int mOverlapEntityId[Q_MAX_OVERLAPS];
        // Overlapping collision components' types in this frame.
        int mOverlapComponentType[Q_MAX_OVERLAPS];

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
            mOverlapEntityId[mNumOverlaps] = aOtherEntityId;
            mOverlapComponentType[mNumOverlaps] = aOtherComponentType;
            ++mNumOverlaps;
        }
        void clear_overlaps()
        {
            for (int i = 0; i < mNumOverlaps; ++i)
            {
                mOverlapEntityId[mNumOverlaps] = -1;
                mOverlapComponentType[mNumOverlaps] = -1;
            }
            mNumOverlaps = 0;
        }
    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, CollisionSphereComponent &collisionSphere) {
        inStream
        >> collisionSphere.mLocalOffset[0] >> collisionSphere.mLocalOffset[1]
        >> collisionSphere.mLocalOffset[2] >> collisionSphere.mRadius
        >> collisionSphere.mCollisionObjectId >> collisionSphere.mNumOverlaps;
        for (int i = 0; i < Q_MAX_OVERLAPS; i++) {
            inStream >> collisionSphere.mOverlapEntityId[i];
        }
        for (int i = 0; i < Q_MAX_OVERLAPS; i++) {
            inStream >> collisionSphere.mOverlapComponentType[i];
        }

        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, const CollisionSphereComponent &collisionSphere) {
        outStream
        << collisionSphere.mLocalOffset[0] << collisionSphere.mLocalOffset[1]
        << collisionSphere.mLocalOffset[2] << collisionSphere.mRadius
        << collisionSphere.mCollisionObjectId << collisionSphere.mNumOverlaps;
        for (int i = 0; i < Q_MAX_OVERLAPS; i++) {
            outStream << collisionSphere.mOverlapEntityId[i];
        }
        for (int i = 0; i < Q_MAX_OVERLAPS; i++) {
            outStream << collisionSphere.mOverlapComponentType[i];
        }
        
        return outStream;
    }
}