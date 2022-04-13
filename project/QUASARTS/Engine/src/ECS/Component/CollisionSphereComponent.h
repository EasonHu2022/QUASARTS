#pragma once

// Local includes:
#include "ECS/ECS-Common.h"


namespace Engine {
    struct QS_API CollisionSphereComponent {
        // Local offset
        float x;
        float y;
        float z;
        // Collision sphere radius (centred on local offset)
        float radius;
        // ID of corresponding collision object in the collision world.
        int collision_id;
    };
}