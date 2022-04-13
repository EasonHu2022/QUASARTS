#pragma once

// Library includes:
#include <array>

// Local includes:
#include "ECS/ECS-Common.h"

// Component includes:
#include "ECS/Component/TransformComponent.h"
#include "ECS/Component/MeshComponent.h"
#include "ECS/Component/CollisionSphereComponent.h"

/* Functions:
 * T get_data(unsigned int entityID)
 * void add_data(unsigned int entityID)
 * void add_data(unsigned int entityID, T data)
 * void copy_data(unsigned int copyFrom, unsigned int copyTo)
 * void remove_data(unsigned int entityID)
 * void replace_data(unsigned int entityID, T data)
 * unsigned int entityID_from_data(unsigned int index)
 * unsigned int data_from_entityID(unsigned int entityID) */

/* Members:
 * std::array<T, MAX_ENTITIES> componentData
 * std::array<unsigned int, MAX_ENTITIES> entityIDs
 * unsigned int num_entries */

namespace Engine {
    class QS_API ParentComponentArray {
        public:
        // Destructor:
        virtual ~ParentComponentArray() {}

        // Remove the entry for an Entity from the array:
        virtual void remove_entity(unsigned int entityID) = 0;

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
        T get_data(unsigned int entityID) {
            unsigned int index = data_from_entityID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                // Print a warning:
                std::cerr << "Warning: Entity " << entityID << " not found!" << std::endl;
                T result{};
                return result;
            }
            return componentData[index];
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

            // Replace the data with the entry from the end of the array:
            componentData[index] = componentData[num_entries];
            entityIDs[index] = entityIDs[num_entries];
            
            // Now the array is packed again. Decrement num_entries:
            num_entries--;
        }

        // Replace data in the component array:
        void replace_data(unsigned int entityID, T data) {
            // Find the index of data:
            unsigned int index = data_from_entityID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                // Print a warning:
                std::cerr << "Warning: Entity " << entityID << " not found!" << std::endl;
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

        // Print out the state of the component array for debugging:
        virtual void print_state() {
            std::cout << "Number of entries: " << num_entries << std::endl;
            std::cout << "Entities:" << std::endl;
            for (int i = 0; i < num_entries; i++) {
                std::cout << entityIDs[i] << "\t";
            }
            std::cout << std::endl;
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
}