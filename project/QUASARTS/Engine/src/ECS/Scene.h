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

            componentArrays.push_back(array_transform);
            componentArrays.push_back(array_mesh);
            componentArrays.push_back(colis_sp);
            componentArrays.push_back(array_mat);
            componentArrays.push_back(array_light);
            componentArrays.push_back(array_script);
            componentArrays.push_back(array_camera);
        }

        // Destructor:
        ~Scene() {
            // Destroy Component arrays:
            for (int i = 0; i < componentArrays.size(); i++) {
                delete componentArrays[i];
            }
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
    };
}