#pragma once

// Library includes:
#include <array>

// Local includes:
#include "ECS/ECS-Common.h"

// Component includes:
#include "ECS/Component/TransformComponent.h"
#include "ECS/Component/CameraComponent.h"
#include "ECS/Component/MeshComponent.h"
#include "ECS/Component/MaterialComponent.h"
#include "ECS/Component/CollisionSphereComponent.h"
#include "ECS/Component/LightComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "ECS/Component/OrbitComponent.h"
#include "ECS/Component/HealthComponent.h"
#include "ECS/Component/WeaponComponent.h"
#include "ECS/Component/ParticleComponent.h"
#include "ECS/Component/EnemyComponent.h"
#include "ECS/Component/EnemySpawnComponent.h"
#include "ECS/Component/ProjectileComponent.h"
#include "ECS/Component/AudioComponent.h"

/* Process for adding new Component types:
 * #include the source file here.
 * Add a template class declaration here (at the bottom).
 * Add a new definition in ECS-Common.h.
 * Update NUM_COMPONENT_TYPES in ECS-Common.h
 * Add a ComponentArray initialization to the Scene.h constructor
 * Add << and >> operators to the component (see existing components) */

/* IMPORTANT NOTE for << >> operators for Components:
 * Please do not save or load raw pointers, for obvious reasons.
 * If your Component requires a non-null pointer, please add 
 * functionality to the initialize_components function. */

namespace Engine {
    class QS_API ParentComponentArray {
        public:
        // Destructor:
        virtual ~ParentComponentArray() {}

        // Remove the entry for an Entity from the array:
        virtual void remove_entity(unsigned int entityID) = 0;

        // Clear all component array data:
        virtual void clear_component_data() = 0;

        // Print out the state of the component array for debugging:
        virtual void print_state() = 0;
    };

    template <typename T>
    class QS_API ComponentArray : public ParentComponentArray {
        public:
        // Constructor:
        ComponentArray() {
            num_entries = 0;
        }

        // Get a data element from the array:
        T *get_data(unsigned int entityID) {
            unsigned int index = data_from_entityID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                // Print a warning:
                QERROR("Function ComponentArray::get_data(): Error: Entity {0} not found!", entityID);
                return nullptr;
            }
            return &(componentData[index]);
        }

        // Add data (zero initialized) to the component array:
        void add_data(unsigned int entityID) {
            unsigned int index = data_from_entityID(entityID);
            if (index != TOO_MANY_ENTITIES) { return; }
            T data{};
            componentData[num_entries] = data;
            entityIDs[num_entries] = entityID;
            num_entries++;
        }

        // Add data to the component array:
        void add_data(unsigned int entityID, T data) {
            unsigned int index = data_from_entityID(entityID);
            if (index != TOO_MANY_ENTITIES) {
                // Replace the data instead of adding it:
                replace_data(entityID, data);
                return;
            }
            componentData[num_entries] = data;
            entityIDs[num_entries] = entityID;
            num_entries++;
        }

        // Copy data from one entity to another:
        void copy_data(unsigned int copyFrom, unsigned int copyTo) {
            // Check that the copyFrom entity exists:
            unsigned int index = data_from_entityID(copyFrom);
            if (index != TOO_MANY_ENTITIES) {
                // If the copyFrom entity doesn't exist, add uninitialized data:
                add_data(copyTo);
                return;
            }
            // Copy the data:
            add_data(copyTo, componentData[index]);
        }

        // Remove data from the component array:
        void remove_data(unsigned int entityID) {
            // Find the index of data:
            unsigned int index = data_from_entityID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                // No data for the entity here, just leave:
                return;
            }

            // Decrement num_entries:
            num_entries--;

            // Replace the data with the entry from the end of the array:
            componentData[index] = componentData[num_entries];
            entityIDs[index] = entityIDs[num_entries];
            // The array is now packed again.
        }

        // Replace data in the component array:
        void replace_data(unsigned int entityID, T data) {
            // Find the index of data:
            unsigned int index = data_from_entityID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                // Print a warning:
                QERROR("Function ComponentArray::replace_data(): Error: Entity {0} not found!", entityID);
                return;
            }

            // Replace the data at the index:
            componentData[index] = data;
        }

        // Get entity ID for data index:
        unsigned int entityID_from_data(unsigned int index) {
            return entityIDs[index];
        }

        // Get data index from entity ID:
        unsigned int data_from_entityID(unsigned int entityID) {
            // Find the index of data:
            unsigned int index = TOO_MANY_ENTITIES;
            for (unsigned int i = 0; i < num_entries; i++) {
                if (entityIDs[i] == entityID) {
                    index = i;
                    break;
                }
            }
            return index;
        }

        // Remove the entry for an Entity from the array:
        virtual void remove_entity(unsigned int entityID) override {
            remove_data(entityID);
        }

        // Clear all component array data:
        virtual void clear_component_data() {
            num_entries = 0;
        }

        // Print out the state of the component array for debugging:
        virtual void print_state() {
            QDEBUG("************************");
            QDEBUG("* Component array info *");
            QDEBUG("************************");
            QDEBUG("Number of entries: {0}", num_entries);
            QDEBUG("Entities:");
            if (num_entries > 0) {
                std::string entityList = std::to_string(entityIDs[0]);
                for (int i = 1; i < num_entries; i++) {
                    entityList += ", " + std::to_string(entityIDs[i]);
                }
                QDEBUG(entityList);
            } else {
                QDEBUG("None");
            }
            QDEBUG("************************");
        }

        private:
        // Component data:
        std::array<T, MAX_ENTITIES> componentData;

        // Correspondence to entities:
        std::array<unsigned int, MAX_ENTITIES> entityIDs;

        // Number of elements currently in the array:
        unsigned int num_entries;
    };

    // Declarations of type-specific Components, for the DLL:
    template class QS_API ComponentArray<TransformComponent>;
    template class QS_API ComponentArray<MeshComponent>;
    template class QS_API ComponentArray<CollisionSphereComponent>;
    template class QS_API ComponentArray<ScriptComponent>;
    template class QS_API ComponentArray<CameraComponent>; 
    template class QS_API ComponentArray<MaterialComponent>;
    template class QS_API ComponentArray<LightComponent>;
    template class QS_API ComponentArray<OrbitComponent>;
    template class QS_API ComponentArray<HealthComponent>;
    template class QS_API ComponentArray<WeaponComponent>;
    template class QS_API ComponentArray<ParticleComponent>;
    template class QS_API ComponentArray<EnemyComponent>;
    template class QS_API ComponentArray<EnemySpawnComponent>;
    template class QS_API ComponentArray<ProjectileComponent>;
    template class QS_API ComponentArray<AudioComponent>;
}