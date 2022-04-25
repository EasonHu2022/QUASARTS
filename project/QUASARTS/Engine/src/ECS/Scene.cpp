#pragma once

#include "Scene.h"
#include "ECS/ECSManager.h"

namespace Engine {

        // Constructor:
        Scene::Scene() {
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
        Scene::~Scene() {
            // Destroy Component arrays:
            for (int i = 0; i < componentArrays.size(); i++) {
                delete componentArrays[i];
            }
        }

        void Scene::init()
        {
            create_camera();
        }

       
       

  
        void Scene::create_camera()
        {
            unsigned int entityID = ECSManager::Instance()->create_entity();
            ECSManager::Instance()->set_entityName(entityID, "camera");
            set_camera(entityID);

            ECSManager::Instance()->create_component<TransformComponent>(entityID, COMPONENT_TRANSFORM);
            TransformComponent transform;
            transform.position = { 0.0f,0.1f, 2.0f };
            transform.rotation = { 0.0f,-90.0f,0.0f };
            ECSManager::Instance()->replace_component(entityID, COMPONENT_TRANSFORM, transform);
            ECSManager::Instance()->create_component<CameraComponent>(entityID, COMPONENT_CAMERA);
        }

        int Scene::get_camera() { return this->camera; }
        void Scene::set_camera(int _camera) { camera = _camera; }
    };
