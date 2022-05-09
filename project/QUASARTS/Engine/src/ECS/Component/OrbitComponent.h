#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

// Engine includes
#include "glm/vec3.hpp"


namespace Engine {

    struct QS_API OrbitComponent
    {
        // ID of the entity which this component's parent entity will orbit around.
        int mPrimaryEntityId;

        // Orbit parameters
        float mOrbitPeriod;         // Time to complete one orbit.

        // Orbit basis.
        glm::vec3 mAxisNormal;      // Direction of the normal vector of the orbit plane.
        glm::vec3 mAxisX;           // Direction of semimajor axis - lies along the apse line, perpendicular to the normal.
        glm::vec3 mAxisY;           // Direction of semiminor axis - right-handed cross-product of the normal and x-axis (in that order).

        // State tracking
        float mTrueAnom;            // True anomaly - the current shortest angle in the orbital plane between the satellite and the positive x-axis. Defines satellite's place in orbit.
        float mDistance;            // Current distance from primary to satellite.
        glm::vec3 mRelativePos;     // Current separation vector of satellite from primary - this entity's world position relative to the primary entity.


        //// Orbit parameters
        //float mGrav;                // Gravitational parameter - sum of satellite and primary masses multiplied by the gravitational constant.
        //glm::vec3 mSraMomentum;     // Specific relative angular momentum.
        //glm::vec3 mEccentricity;    // Controls orbit shape.
        //float mDistPeriapse;        // Distance from primary to periapse - closest point of orbit.
        //float mDistApoapse;         // Distance from primary to apoapse - farthest point of orbit.

        //// Ellipse axis lengths.
        //float mLenSemiMinor;           // Distance from ellipse centre to nearest point(s) on the curve.
        //float mLenSemiMajor;           // Distance from ellipse centre to farthest point(s) on the curve.


        OrbitComponent() :
            mPrimaryEntityId(-1)
        { }

        /// <summary>
        /// Set the orbit primary - the entity which this component's parent entity will orbit around.
        /// </summary>
        /// <param name="aPrimaryEntityId">ID of the entity to orbit around.</param>
        /// <param name="aSatOffset">Relative position of the orbiting entity.</param>
        void set_primary(unsigned int const aPrimaryEntityId)
        {
            
        }


        void init_orbit(glm::vec3 aRelativeWorldPos)
        {

        }

    };

    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, OrbitComponent& orbit) {
        inStream >> orbit.mPrimaryEntityId >> orbit.mOrbitPeriod >> orbit.mAxisNormal[0]
            >> orbit.mAxisNormal[1] >> orbit.mAxisNormal[2] >> orbit.mAxisX[0]
            >> orbit.mAxisX[1] >> orbit.mAxisX[2] >> orbit.mAxisY[0] >> orbit.mAxisY[1]
            >> orbit.mAxisY[2];
        
        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, OrbitComponent const& orbit) {
        outStream << orbit.mPrimaryEntityId << " " << orbit.mOrbitPeriod << " "
            << orbit.mAxisNormal[0] << " " << orbit.mAxisNormal[1] << " " << orbit.mAxisNormal[2]
            << " " << orbit.mAxisX[0] << " " << orbit.mAxisX[1] << " " << orbit.mAxisX[2] << " "
            << orbit.mAxisY[0] << " " << orbit.mAxisY[1] << " " << orbit.mAxisY[2];

        return outStream;
    }
}