#pragma once

// Library includes:
#include <array>

// Local includes:
#include "Core/Core.h"

namespace Engine {
    class QS_API ParentComponentArray {
        public:
        // Destructor:
        virtual ~ParentComponentArray();

        // Add data to the component array:
        template <typename T>
        void add_data(T data, unsigned int entityID);

        // Add a data entry (uninitialized) to the component array:
        virtual void add_data(unsigned int entityID);

        // Copy data from one entity to another:
        virtual void copy_data(unsigned int copyFrom, unsigned int copyTo);

        // Remove data from the component array:
        virtual void remove_data(unsigned int entityID) = 0;
    };

    template <typename T>
    class QS_API ComponentArray : public ParentComponentArray {
        public:
        // Constructor:
        ComponentArray() {
            num_entries = 0;
        }

        // Add data to the component array:
        void add_data(T data, unsigned int entityID) override {
            unsigned int index = data_from_entityID(entityID);
            if (index != (MAX_ENTITIES + 1)) {
                // Replace the data instead of adding it:
                replace_data(data, entityID);
                return;
            }
            componentData[num_entries] = data;
            entityIDs[num_entries] = entityID;
            num_entries++;
        }

        // Add a data entry (uninitialized) to the component array:
        void add_data(unsigned int entityID) override {
            unsigned int index = data_from_entityID(entityID);
            if (index != (MAX_ENTITIES + 1)) { return; }
            entityIDs[num_entries] = entityID;
            num_entries++;
        }

        // Copy data from one entity to another:
        void copy_data(unsigned int copyFrom, unsigned int copyTo) override {
            // Check that the copyFrom entity exists:
            unsigned int index = data_from_entityID(copyFrom);
            if (index != (MAX_ENTITIES + 1)) {
                // If the copyFrom entity doesn't exist, add uninitialized data:
                add_data(copyTo);
                return;
            }
            // Copy the data:
            add_data(componentData[index], copyTo);
        }

        // Remove data from the component array:
        void remove_data(unsigned int entityID) {
            // Find the index of data:
            unsigned int index = data_from_entityID(entityID);
            if (index == (MAX_ENTITIES + 1)) {
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
        void replace_data(T data, unsigned int entityID) {
            // Find the index of data:
            unsigned int index = data_from_entityID(entityID);
            if (index == (MAX_ENTITIES + 1)) {
                // No data for the entity here, just leave:
                return;
            }

            // Replace the data at the index:
            componentData[index] = data;
        }

        // Get pointer to component array:
        std::array<T, MAX_ENTITIES> *get_componentData() {
            return &componentData;
        }

        // Get entity ID for data index:
        unsigned int entityID_from_data(unsigned int index) {
            return entityIDs[index];
        }

        // Get data index from entity ID:
        unsigned int data_from_entityID(unsigned int entityID) {
            // Find the index of data:
            unsigned int index = MAX_ENTITIES + 1;
            for (int i = 0; i < num_entries; i++) {
                if (entityIDs[i] == entityID) {
                    index = i;
                    break;
                }
            }
            return index;
        }

        private:
        // Component data:
        std::array<T, MAX_ENTITIES> componentData;

        // Correspondence to entities:
        std::array<unsigned int, MAX_ENTITIES> entityIDs;

        // Number of elements currently in the array:
        unsigned int num_entries;
    };
}