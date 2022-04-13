#pragma once

// Library includes:
#include <cstdint>

// Local includes:
#include "Core/Core.h"

namespace Engine {
    // Maximum number of component types allowed:
    #define MAX_COMPONENT_TYPES 64

    // Maximum number of entities allowed:
    #define MAX_ENTITIES 1024
    #define TOO_MANY_ENTITIES 1025

    // Component types:
    #define COMPONENT_TRANSFORM         0
    #define COMPONENT_MESH              1
    #define COMPONENT_FAMILY            2
    #define COMPONENT_COLLISION_SPHERE  3

    // Systems:
    #define SYSTEM_EXAMPLE 0

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