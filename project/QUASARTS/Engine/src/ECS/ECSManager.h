#pragma once

// Library includes:
#include <vector>
#include <map>
#include <iostream>

// Local includes:
#include "Core/Core.h"
#include "Core/IManager.h"
#include "ECS/ECS-Common.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/ComponentArray.h"
#include "ECS/System/System.h"
#include "ECS/Scene.h"

namespace Engine {
    class QS_API ECSManager : public IManager {
        private:
        // Singleton:
        static ECSManager *instance;
        ECSManager() : scene(nullptr), systems() {}

        public:
        static ECSManager *Instance();
        ~ECSManager();

        // Functions inherited from IManager:
        void init();
		int start();
		void update();
		int stop();
		void release();

        // Get a new Entity ID (the first free one):
        unsigned int get_free_entity_ID();

        // Create a new Entity and add it to the vector:
        unsigned int create_entity();

        // Destroy an Entity:
        void destroy_entity(unsigned int entityID);

        template <typename T>
        void create_component(unsigned int entityID, unsigned int componentType, T data) {
            // Update the component mask of the Entity:
            int matched = 0;
            unsigned int entity_index;
            for (int i = 0; i < entity_ID_match.size(); i++) {
                if (entity_ID_match[i] == entityID) {
                    entities[i].add_component_type(componentType);
                    entity_index = i;
                    matched = 1;
                    break;
                }
            }
            // If no match was found, print a warning and leave:
            if (matched == 0) {
                std::cerr << "Warning: no match was found for entity " << entityID << "!" << std::endl;
                return;
            }
            // Update each System:
            for (int i = 0; i < systems.size(); i++) {
                systems[i]->test_entity(entities[entity_index].get_componentMask(), entityID);
            }
            // Add the data to the relevant component array:
            ComponentArray<T> *compArray = (ComponentArray<T> *)componentArrays[componentType];
            compArray->add_data(entityID, data);
        }

        // Add a new Component with no specific data:
        template <typename T>
        void create_component(unsigned int entityID, unsigned int componentType) {
            // Update the component mask of the Entity:
            int matched = 0;
            unsigned int entity_index;
            for (int i = 0; i < entity_ID_match.size(); i++) {
                if (entity_ID_match[i] == entityID) {
                    entities[i].add_component_type(componentType);
                    entity_index = i;
                    matched = 1;
                    break;
                }
            }
            // If no match was found, print a warning and leave:
            if (matched == 0) {
                std::cerr << "Warning: no match was found for entity " << entityID << "!" << std::endl;
                return;
            }
            // Update each System:
            for (int i = 0; i < systems.size(); i++) {
                systems[i]->test_entity(entities[entity_index].get_componentMask(), entityID);
            }
            // Add the data to the relevant component array:
            ComponentArray<T> *compArray = (ComponentArray<T> *)componentArrays[componentType];
            compArray->add_data(entityID);
        }

        // Replace Component data in the Component array:
        template <typename T>
        void replace_component(unsigned int entityID, unsigned int componentType, T data) {
            // Check that the Entity exists:
            int matched = 0;
            unsigned int entity_index;
            for (int i = 0; i < entity_ID_match.size(); i++) {
                if (entity_ID_match[i] == entityID) {
                    entity_index = i;
                    matched = 1;
                    break;
                }
            }
            // If no match was found, print a warning and leave:
            if (matched == 0) {
                std::cerr << "Warning: no match was found for entity " << entityID << "!" << std::endl;
                return;
            }
            // Check the Component mask of the Entity:
            quasarts_component_mask mask = {0};
            mask.mask = (uint64_t)1 << componentType;
            if ((entities[entity_index].get_componentMask().mask & mask.mask) != mask.mask) {
                // Add the data instead of replacing it:
                create_component<T>(entityID, componentType, data);
                return;
            }

            // If the Component exists for the Entity, replace the data:
            ComponentArray<T> *compArray = (ComponentArray<T> *)componentArrays[componentType];
            compArray->replace_data(entityID, data);
        }

        // Get a Component from the Component array:
        template <typename T>
        T get_component(unsigned int entityID, unsigned int componentType) {
                ComponentArray<T> *compArray = (ComponentArray<T> *)componentArrays[componentType];
                return compArray->get_data(entityID);
        }

        // Copy a component from one Entity to another:
        template <typename T>
        void copy_component(unsigned int copyFrom, unsigned int copyTo, unsigned int componentType) {
                ComponentArray<T> *compArray = (ComponentArray<T> *)componentArrays[componentType];
                compArray->copy_data(copyFrom, copyTo);
        }

        // Remove a Component from an Entity:
        template <typename T>
        void destroy_component(unsigned int entityID, unsigned int componentType) {
            // Update the component mask of the entity:
            int matched = 0;
            unsigned int entity_index;
            for (int i = 0; i < entity_ID_match.size(); i++) {
                if (entity_ID_match[i] == entityID) {
                    entities[i].remove_component_type(componentType);
                    entity_index = i;
                    matched = 1;
                    break;
                }
            }
            // If no match was found, print a warning and leave:
            if (matched == 0) {
                std::cerr << "Warning: no match was found for entity " << entityID << "!" << std::endl;
                return;
            }
            // Update each System:
            for (int i = 0; i < systems.size(); i++) {
                systems[i]->test_entity(entities[entity_index].get_componentMask(), entityID);
            }
            // Remove the data from the relevant component array:
            ComponentArray<T> *compArray = (ComponentArray<T> *)componentArrays[componentType];
            compArray->remove_data(entityID);
        }

        // Get a pointer to an Entity:
        Entity *get_entity(unsigned int EntityID);

        // Get the list of Entity IDs:
        quasarts_entity_ID_mask *get_entityIDs();

        // Register a System with the Manager:
        void register_system(unsigned int systemType, System *system);

        // Deregister a System with the Manager:
        void deregister_system(unsigned int systemType);

        // Set the pointer to the current scene:
        void set_scene(Scene *scene_ptr);

        // Save the whole scene to file:
        bool save_scene(char *filename);

        // Load a scene from file:
        bool load_scene(char *filename);

        // Print Entity information for debugging purposes:
        void print_entities();

        // Print out Component array information:
        void print_componentArray_info(unsigned int componentType);

        private:
        // Currently loaded scene:
        Scene *scene;

        // Systems:
        std::map<unsigned int, System *> systems;  // For iterating over Systems easily.
    };
}