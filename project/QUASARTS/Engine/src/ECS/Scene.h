#pragma once

// Library includes:
#include <vector>
#include <set>
#include <map>
#include <string>

// Local includes:
#include "ECS/ECS-Common.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/ComponentArray.h"

namespace Engine {
    class QS_API Scene {
        friend class QS_API ECSManager;
        public:
        // Constructor:
        Scene() {
            // Set the scene name to default:
            name = "Default Scene";

            // Zero-initialize the entity IDs:
            entity_IDs = {0};

            // Add Component arrays:
            ComponentArray<TransformComponent> *array_transform = new ComponentArray<TransformComponent>;
            ComponentArray<MeshComponent> *array_mesh = new ComponentArray<MeshComponent>;
            ComponentArray<CollisionSphereComponent>* colis_sp = new ComponentArray<CollisionSphereComponent>;
            ComponentArray<MaterialComponent>* array_mat = new ComponentArray<MaterialComponent>;
            ComponentArray<LightComponent>* array_light = new ComponentArray<LightComponent>;
            ComponentArray<ScriptComponent>* array_script = new ComponentArray<ScriptComponent>;
            ComponentArray<CameraComponent>* array_camera = new ComponentArray<CameraComponent>;
            ComponentArray<OrbitComponent>* array_orbit = new ComponentArray<OrbitComponent>;

            componentArrays.push_back(array_transform);
            componentArrays.push_back(array_mesh);
            componentArrays.push_back(colis_sp);
            componentArrays.push_back(array_mat);
            componentArrays.push_back(array_light);
            componentArrays.push_back(array_script);
            componentArrays.push_back(array_camera);
            componentArrays.push_back(array_orbit);
        }

        // Destructor:
        ~Scene() {
            // Destroy Component arrays:
            for (int i = 0; i < componentArrays.size(); i++) {
                delete componentArrays[i];
            }
        }

        // Create scene camera:
        void create_camera(unsigned int cameraID) {
            // Create a new Entity with a name:
            Entity camera_entity = Entity(cameraID);
            camera_entity.set_name("Camera");

            // Add relevant Components to the camera:
            TransformComponent transform;
            transform.position = { 0.0f, 1.0f, 5.0f };
            transform.rotation = { 0.0f,-90.0, 0.0f };
            camera_entity.add_component_type(COMPONENT_CAMERA);
            camera_entity.add_component_type(COMPONENT_TRANSFORM);

            // Add the data to the relevant Component arrays:
            ComponentArray<CameraComponent> *cameraCompArray =
                                (ComponentArray<CameraComponent> *)componentArrays[COMPONENT_CAMERA];
            cameraCompArray->add_data(cameraID);
            ComponentArray<TransformComponent> *transformCompArray =
                                (ComponentArray<TransformComponent> *)componentArrays[COMPONENT_TRANSFORM];
            transformCompArray->add_data(cameraID, transform);

            // Update scene data:
            camera = cameraID;
            entities.push_back(camera_entity);
            entity_ID_match.push_back(cameraID);
            entity_IDs.mask[cameraID] = 1;
            parents.push_back(TOO_MANY_ENTITIES);
            children.push_back({});
        }

        void clear_data() {
            // Clear Component array data:
            for (int i = 0; i < componentArrays.size(); i++) {
                componentArrays[i]->clear_component_data();
            }

            // Reset name:
            name = "Default Scene";

            // Clear Entity data:
            entities.clear();
            entity_ID_match.clear();

            // Clear Entity groups:
            entity_groups.clear();

            // Clear Entity ID mask:
            for (int i = 0; i < MAX_ENTITIES; i++) {
                entity_IDs.mask[i] = 0;
            }

            // Clear parent-child relationships:
            children.clear();
            parents.clear();

            // Reset the camera Entity:
            camera = TOO_MANY_ENTITIES;
        }

        private:
        // Scene name:
        std::string name;

        // Entities:
        std::vector<Entity> entities;
        std::vector<unsigned int> entity_ID_match;

        // Groups of Entities:
        std::map<std::string, std::set<unsigned int>> entity_groups;

        // Component arrays:
        std::vector<ParentComponentArray *> componentArrays;

        // Bit mask (array) of entity IDs that are in use (0 = free, 1 = used):
        quasarts_entity_ID_mask entity_IDs;

        // Parents and children:
        std::vector<std::set<unsigned int>> children;
        std::vector<unsigned int> parents;

        // Scene camera:
        unsigned int camera = TOO_MANY_ENTITIES;
    };
}