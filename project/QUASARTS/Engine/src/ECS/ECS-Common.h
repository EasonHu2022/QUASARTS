#pragma once

// Library includes:
#include <cstdint>

// Component includes:
#include "ECS/Component/TransformComponent.h"
#include "ECS/Component/MeshComponent.h"

namespace Engine {
    // Class declarations for the whole ECS:
    class QS_API Entity;
    class QS_API ParentComponentArray;
    class QS_API System;
    class QS_API ECSManager;

    // Maximum number of component types allowed:
    #define MAX_COMPONENT_TYPES 64

    // Maximum number of entities allowed:
    #define MAX_ENTITIES 1024

    // Component types:
    #define COMPONENT_TRANSFORM 0
    #define COMPONENT_MESH 1

    // Bit field structure for component types:
    struct quasarts_component_mask {
        uint64_t mask : MAX_COMPONENT_TYPES;
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