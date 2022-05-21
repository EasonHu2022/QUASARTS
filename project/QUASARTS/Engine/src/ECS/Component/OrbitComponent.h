#pragma once

// Local includes:
#include "ECS/ECS-Common.h"

// Engine includes
#include "glm/vec3.hpp"

// std lib includes
#include <sstream>


namespace Engine {

    struct QS_API OrbitComponent
    {
        // True if this component should be included in orbit simulation updates, false otherwise.
        bool mActive;

        // ID of the entity which this component's parent entity will orbit around.
        int mPrimaryEntityId;

        // Orbit parameters.
        float mOrbitPeriod;         // Time to complete one orbit.
        float mDistPeriapse;        // Distance from primary to periapse - closest point of orbit.

        // Orbit basis.
        glm::vec3 mAxisNormal;      // Direction of the normal vector of the orbit plane.
        glm::vec3 mAxisX;           // Direction of semimajor axis - lies along the apse line, perpendicular to the normal.
        glm::vec3 mAxisY;           // Direction of semiminor axis - right-handed cross-product of the normal and x-axis (in that order).

        // State tracking.
        float mTrueAnom;            // True anomaly - the current shortest angle in the orbital plane between the satellite and the positive x-axis. Defines satellite's place in orbit.
        float mDistance;            // Current distance from primary to satellite.
        glm::vec3 mRelativePos;     // Current separation vector of satellite from primary - this entity's world position relative to the primary entity.
        
        // State reading.
        float mTrueAnomDeg;


        //// Orbit parameters
        //float mGrav;                // Gravitational parameter - sum of satellite and primary masses multiplied by the gravitational constant.
        //glm::vec3 mSraMomentum;     // Specific relative angular momentum.
        //glm::vec3 mEccentricity;    // Controls orbit shape.
        //float mDistApoapse;         // Distance from primary to apoapse - farthest point of orbit.

        //// Ellipse axis lengths.
        //float mLenSemiMinor;           // Distance from ellipse centre to nearest point(s) on the curve.
        //float mLenSemiMajor;           // Distance from ellipse centre to farthest point(s) on the curve.


        // Default normal, used until the user defines a new normal.
        static constexpr glm::vec3 defNormal = glm::vec3(0, 1, 0);

        // Alternative positive-Y axis, used if the user-defined X axis lies on the default normal.
        static constexpr glm::vec3 altY = glm::vec3(1, 0, 0);


        OrbitComponent() 
            : mActive(false)
            , mPrimaryEntityId(-1)
            , mOrbitPeriod(0)
            , mDistPeriapse(0)
            , mAxisNormal(defNormal)
            , mAxisX(0,0,0)
            , mAxisY(0,0,0)
            , mTrueAnom(0)
            , mDistance(0)
            , mRelativePos(0,0,0)
            , mTrueAnomDeg(0)
        { }


        // Util //

        void clear()
        {
            mActive = false;
            mPrimaryEntityId = -1;
            mDistance = mTrueAnom = mDistPeriapse = mOrbitPeriod = 0;
            mRelativePos = mAxisY = mAxisX = glm::vec3(0, 0, 0);
            mAxisNormal = defNormal;
        }


        // Debug //

        static std::string vec3_tostring(glm::vec3 const& vector, std::string const& separator = " ")
        {
            std::ostringstream ostr;
            ostr << vector.x << separator << vector.y << separator << vector.z;
            return ostr.str();
        }

        std::string to_string()
        {
            std::ostringstream ostr;
            ostr << "Orbit: " << "primary: " << mPrimaryEntityId << ", periapse: " << mDistPeriapse << ", period: " << mOrbitPeriod
                << "\n- X: (" << vec3_tostring(mAxisX) << "), Y: (" << vec3_tostring(mAxisY) << "), Normal: (" << vec3_tostring(mAxisNormal) << ")";
            return ostr.str();
        }

    };


    // Input stream operator:
    inline std::istream & operator >> (std::istream &inStream, OrbitComponent& orbit) {
        inStream >> orbit.mPrimaryEntityId >> orbit.mOrbitPeriod
            >> orbit.mAxisNormal[0] >> orbit.mAxisNormal[1] >> orbit.mAxisNormal[2];
        
        return inStream;
    }
            
    // Output stream operator:
    inline std::ostream & operator << (std::ostream &outStream, OrbitComponent const& orbit) {
        outStream << orbit.mPrimaryEntityId << " " << orbit.mOrbitPeriod << " "
            << orbit.mAxisNormal[0] << " " << orbit.mAxisNormal[1] << " " << orbit.mAxisNormal[2];

        return outStream;
    }


    inline std::ostream& operator << (std::ostream& outStream, glm::vec3 const& vector)
    {
        outStream << vector.x << " " << vector.y << " " << vector.z;
        return outStream;
    }

}