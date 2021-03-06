#pragma once

// Library includes:
#include <cstdint>
#include <iostream>

// Local includes:
#include "Core/Core.h"

namespace Engine {
    // Maximum length of a Scene file line:
    #define MAX_SCENE_LINE_LENGTH 150

    // Maximum number of component types allowed:
    #define MAX_COMPONENT_TYPES 64
    #define NUM_COMPONENT_TYPES 18

    // Maximum number of entities allowed:
    #define MAX_ENTITIES 1024
    #define TOO_MANY_ENTITIES 1025

    // Component types:
    #define COMPONENT_TRANSFORM         0
    #define COMPONENT_MESH              1
    #define COMPONENT_COLLISION_SPHERE  2
    #define COMPONENT_MATERIAL          3
    #define COMPONENT_LIGHTING          4
    #define COMPONENT_SCRIPT            5
    #define COMPONENT_CAMERA            6
    #define COMPONENT_ORBIT             7
    #define COMPONENT_HEALTH            8
    #define COMPONENT_WEAPON            9
    #define COMPONENT_PARTICLE          10
    #define COMPONENT_ENEMY             11
    #define COMPONENT_ENEMY_SPAWNER     12
    #define COMPONENT_PROJECTILE        13
    #define COMPONENT_AUDIO             14
    #define COMPONENT_LIFESPAN          15
    #define COMPONENT_ARMOUR            16
    #define COMPONENT_UI_ELEMENT        17

    // System types:
    #define SYSTEM_SCRIPT       0
    #define SYSTEM_RENDER       1
    #define SYSTEM_PHYSICS      2
    #define SYSTEM_COLLISION    3
    #define SYSTEM_AUDIO        4
    #define SYSTEM_ORBIT        5
    #define SYSTEM_PARTICLE     6
    #define SYSTEM_COMBAT       7
    #define SYSTEM_CAMERA       8
    #define SYSTEM_UI           9

    // Bit field structure for component types:
    struct quasarts_component_mask {
        uint64_t mask : MAX_COMPONENT_TYPES;
    };

    // "Bit field" (in this case an array) structure for entity IDs:
    struct quasarts_entity_ID_mask {
        unsigned short mask[MAX_ENTITIES];
    };
}