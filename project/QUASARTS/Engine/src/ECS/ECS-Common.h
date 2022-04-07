#pragma once

// Component includes:
//#include "ECS/Component/TransformComponent.h"
//#include "ECS/Component/MeshComponent.h"

namespace Engine {
    // Class declarations for the whole ECS:
    class QS_API Entity;
    class QS_API ParentComponentArray;
    class QS_API System;
    class QS_API ECSManager;

    /* Maximum number of component types allowed. Note that if this changes to a
     * higher value, the type within the component mask will need to be changed.
     * Since it's currently a long long, there isn't anywhere to go from there.
     * It will have to be changed to an array like the entity ID mask. Then the
     * function that compares component signatures will have to be changed. */
    #define MAX_COMPONENT_TYPES 64

    // Maximum number of entities allowed:
    #define MAX_ENTITIES 1024

    // Component types:
    #define COMPONENT_TRANSFORM 0
    #define COMPONENT_MESH 1

    #define COMPONENT_TYPES     TransformComponent, \
                                MeshComponent

    // Bit field structure for component types:
    struct quasarts_component_mask {
        unsigned long long mask : MAX_COMPONENT_TYPES;
    };

    // "Bit field" (in this case an array) structure for entity IDs:
    struct quasarts_entity_ID_mask {
        unsigned short mask[MAX_ENTITIES];
    };

    // Struct to hold component indices:
    struct quasarts_component_indices {
        unsigned int indices[MAX_COMPONENT_TYPES];
    };
}