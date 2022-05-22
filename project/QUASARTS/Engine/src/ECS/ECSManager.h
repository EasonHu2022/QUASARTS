#pragma once

// Library includes:
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <sstream>

// Local includes:
#include "Core/Core.h"
#include "Core/IManager.h"
#include "ECS/ECS-Common.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/ComponentArray.h"
#include "ECS/System/System.h"
#include "ECS/Scene.h"

// Extras:
#include "Entity-Stress-Test.h"

namespace Engine {
    class QS_API ECSManager : public IManager {
        private:
        // Singleton:
        static ECSManager *instance;
        ECSManager() {
            scene = new Scene();
            create_camera();
            current_entity = TOO_MANY_ENTITIES;
            game_running = false;
        }

        public:
        static ECSManager *Instance();
        ~ECSManager() {
            delete scene;
        }

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

        // Get the name of an Entity:
        std::string get_entityName(unsigned int entityID);

        // Change the name of an Entity:
        void set_entityName(unsigned int entityID, std::string name);

        // Destroy an Entity:
        void destroy_entity(unsigned int entityID);

        template <typename T>
        void create_component(unsigned int entityID, unsigned int componentType, T data) {
            // Update the component mask of the Entity:
            unsigned int index = get_index_from_ID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                QERROR("Function ECSManager::create_component(): Error: no match was found for Entity {0}!", entityID);
                return;
            }
            scene->entities[index].add_component_type(componentType);

            // Update each System:
            for (const auto &[key, val] : systems) {
                val->test_entity(this, entityID, scene->entities[index].get_componentMask());
            }

            // Add the data to the relevant Component array:
            ComponentArray<T> *compArray = (ComponentArray<T> *)scene->componentArrays[componentType];
            compArray->add_data(entityID, data);
        }

        // Add a new Component with no specific data:
        template <typename T>
        void create_component(unsigned int entityID, unsigned int componentType) {
            // Update the component mask of the Entity:
            unsigned int index = get_index_from_ID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                QERROR("Function ECSManager::create_component(): Error: no match was found for Entity {0}!", entityID);
                return;
            }
            scene->entities[index].add_component_type(componentType);

            // Update each System:
            for (const auto &[key, val] : systems) {
                val->test_entity(this, entityID, scene->entities[index].get_componentMask());
            }

            // Add the data to the relevant Component array:
            ComponentArray<T> *compArray = (ComponentArray<T> *)scene->componentArrays[componentType];
            compArray->add_data(entityID);
        }

        // Replace Component data in the Component array:
        template <typename T>
        void replace_component(unsigned int entityID, unsigned int componentType, T data) {
            // Check that the Entity exists:
            unsigned int index = get_index_from_ID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                QERROR("Function ECSManager::replace_component(): Error: no match was found for Entity {0}!", entityID);
                return;
            }

            // Check the Component mask of the Entity:
            quasarts_component_mask mask = {0};
            mask.mask = (uint64_t)1 << componentType;
            if ((scene->entities[index].get_componentMask()->mask & mask.mask) != mask.mask) {
                // Add the data instead of replacing it:
                create_component<T>(entityID, componentType, data);
                return;
            }

            // If the Component exists for the Entity, replace the data:
            ComponentArray<T> *compArray = (ComponentArray<T> *)scene->componentArrays[componentType];
            compArray->replace_data(entityID, data);
        }

        // Get a Component from the Component array:
        template <typename T>
        T *get_component(unsigned int entityID, unsigned int componentType) {
                ComponentArray<T> *compArray = (ComponentArray<T> *)scene->componentArrays[componentType];
                return compArray->get_data(entityID);
        }

        // Copy a Component from one Entity to another:
        template <typename T>
        void copy_component(unsigned int copyFrom, unsigned int copyTo, unsigned int componentType) {
                ComponentArray<T> *compArray = (ComponentArray<T> *)scene->componentArrays[componentType];
                compArray->copy_data(copyFrom, copyTo);
        }

        // Remove a Component from an Entity:
        template <typename T>
        void destroy_component(unsigned int entityID, unsigned int componentType) {
            // Update the component mask of the entity:
            unsigned int index = get_index_from_ID(entityID);
            if (index == TOO_MANY_ENTITIES) {
                QERROR("Function ECSManager::destroy_component(): Error: no match was found for Entity {0}!", entityID);
                return;
            }
            scene->entities[index].remove_component_type(componentType);

            // Update each System:
            for (const auto &[key, val] : systems) {
                val->test_entity(this, entityID, scene->entities[index].get_componentMask());
            }

            // Remove the data from the relevant Component array:
            ComponentArray<T> *compArray = (ComponentArray<T> *)scene->componentArrays[componentType];
            compArray->remove_data(entityID);
        }

        // Check if an Entity has a particular type of Component:
        bool has_component(unsigned int entityID, unsigned int componentType);

        // Get all Component types that an Entity has:
        std::vector<unsigned int> get_all_component_types(unsigned int entityID);

        // Add an Entity group:
        void add_entity_group(std::string group_name);

        // Destroy an Entity group (does not destroy Entities in the group):
        void destroy_entity_group(std::string group_name);

        // Add an Entity to a group:
        void add_entity_to_group(std::string group_name, unsigned int entityID);

        // Remove an Entity from a group:
        void remove_entity_from_group(std::string group_name, unsigned int entityID);

        // Destroy all the Entities in a group (does not destroy the group):
        void destroy_entities_in_group(std::string group_name);

        // Get a pointer to an Entity:
        Entity *get_entity(unsigned int EntityID);

        // Get the mask of Entity IDs:
        quasarts_entity_ID_mask get_entity_ID_mask();

        // Get the Entity ID match list:
        std::vector<unsigned int> get_entity_ID_match();

        // Add a child to an Entity:
        void add_child(unsigned int parent, unsigned int child);

        // Remove a child from an Entity:
        void remove_child(unsigned int parent, unsigned int child);

        // Get the children of an Entity:
        std::set<unsigned int> get_children(unsigned int entityID);

        // Get the parent of an Entity:
        unsigned int get_parent(unsigned int entityID);

        // Check if the Entity has a parent:
        bool has_parent(unsigned int entityID);

        // Check if the Entity has children:
        bool has_children(unsigned int entityID);

        // Register a System with the Manager:
        void register_system(unsigned int systemType, System *system);

        // Deregister a System with the Manager:
        void deregister_system(unsigned int systemType);

        // Get the current Entity ID:
        unsigned int get_current_entity();

        // Set the current Entity ID:
        void set_current_entity(unsigned int entityID);

        // Create the scene camera:
        void create_camera();

        // Get the scene camera:
        unsigned int get_camera();

        // Set the scene camera:
        void set_camera(unsigned int cameraID);

        // Get the name of the scene:
        std::string get_scene_name();

        // Set the name of the scene:
        void set_scene_name(std::string name);

        // Create new blank scene:
        void new_scene();

        // Create new blank scene with a name:
        void new_scene(std::string name);

        // Save the whole scene to file:
        bool save_scene(char *filename);

        // Load a scene from file:
        bool load_scene(char *filename);

        // Get an index for an Entity by ID:
        unsigned int get_index_from_ID(unsigned int entityID);

        // Set the value of game_running:
        void set_game_running(bool state);

        // Get the value of game_running:
        bool get_game_running();

        // Get the number of entities in the scene:
        unsigned int get_num_entities();

        // Print Entity information for debugging purposes:
        void print_entities();

        // Print out Component array information:
        void print_componentArray_info(unsigned int componentType);

        // Print out System information:
        void print_system_info(unsigned int systemType);

        // Print out all System information:
        void print_all_system_info();

        private:
        // Currently loaded scene:
        Scene *scene;

        // Currently selected Entity:
        unsigned int current_entity;

        // Systems:
        std::map<unsigned int, System *> systems;  // For iterating over Systems easily.

        // Boolean to tell whether the game is running or not:
        bool game_running;
    };
}