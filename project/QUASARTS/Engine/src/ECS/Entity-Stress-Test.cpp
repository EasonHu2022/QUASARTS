#include "Entity-Stress-Test.h"

namespace Engine
{
    void construct_solar_system()
    {
        QDEBUG("Constructing solar system.");
        // Indices in vector (NOT ENTITY IDS), and texture paths:
        std::vector<unsigned int> solarSystemIDs;
        std::vector<std::string> planetNames = {
            "Sun",
            "Mercury",
            "Venus",
            "Earth",
            "Mars",
            "Jupiter",
            "Saturn",
            "Uranus",
            "Neptune",
            "Moon"
        };

        // Planet scales relative to Jupiter (Sun not accurate):
        std::vector<glm::vec3> planetScales = {
            glm::vec3(2.00),    // Sun.
            glm::vec3(0.10),    // Mercury.
            glm::vec3(0.20),    // Venus.
            glm::vec3(0.25),    // Earth.
            glm::vec3(0.15),    // Mars.
            glm::vec3(1.25),    // Jupiter.
            glm::vec3(1.00),    // Saturn.
            glm::vec3(0.50),    // Uranus.
            glm::vec3(0.50),    // Neptune.
            glm::vec3(0.05)     // Moon.
        };

        // Positions of the planets:
        std::vector<glm::vec3> planetPositions = {
            glm::vec3(0.0, 0.0, -10.0),     // Sun.
            glm::vec3(0.0, 0.0,  -5.0),     // Mercury.
            glm::vec3(0.0, 0.0,  -3.0),     // Venus.
            glm::vec3(0.0, 0.0,  -1.0),     // Earth.
            glm::vec3(0.0, 0.0,   1.0),     // Mars.
            glm::vec3(0.0, 0.0,   4.0),     // Jupiter.
            glm::vec3(0.0, 0.0,   8.0),     // Saturn.
            glm::vec3(0.0, 0.0,  11.0),     // Uranus.
            glm::vec3(0.0, 0.0,  14.0),     // Neptune.
            glm::vec3(0.0, 0.0,  -1.5)      // Moon.
        };

        // Rotation of each planet - to get their textures the right way up:
        glm::vec3 planetRotation = glm::vec3(-90.0, 0.0, 0.0);

        // Planet orbit periods (not accurate):
        std::vector<float> orbitPeriods = {
            0.0,    // Sun (no orbit, just here for indexing purposes).
            10.0,   // Mercury.
            12.5,   // Venus.
            15.0,   // Earth.
            17.5,   // Mars.
            25.0,   // Jupiter.
            35.0,   // Saturn.
            50.0,   // Uranus.
            70.0,   // Neptune.
            7.5     // Moon.
        };

        // Add a light:
        QDEBUG("---> Adding light.");
        {
            unsigned int lightID = ECSManager::Instance()->create_entity();
            ECSManager::Instance()->set_entityName(lightID, "Light");
            ECSManager::Instance()->create_component<LightComponent>(lightID, COMPONENT_LIGHTING);

            TransformComponent *lightTransform = ECSManager::Instance()->get_component
                                            <TransformComponent>(lightID, COMPONENT_TRANSFORM);
            lightTransform->position = planetPositions[0];
        }

        QDEBUG("---> Transforming camera.");
        // Transform the camera to be looking top-down:
        {
            unsigned int cameraID = ECSManager::Instance()->get_camera();
            TransformComponent *cameraTransform = ECSManager::Instance()->get_component
                                            <TransformComponent>(cameraID, COMPONENT_TRANSFORM);
            cameraTransform->position = planetPositions[0];
            cameraTransform->position.y += 13.0;
            cameraTransform->position.z += 16.0;
            cameraTransform->rotation = glm::vec3(-40.0, -90.0, 0.0);
        }

        // Create Entity IDs:
        QDEBUG("---> Creating new Entity IDs for solar system.");
        for (int i = 0; i < 10; i++)
        {
            unsigned int entityID = ECSManager::Instance()->create_entity();
            solarSystemIDs.push_back(entityID);
        }

        // Add Mesh, Material and Orbit Components, and set the scale of the planets:
        QDEBUG("---> Creating Components for solar system Entities.");
        {
            auto path = FileModule::Instance()->get_internal_assets_path();
            std::string vshPath = path + "Shader/DefaultShader.vsh";
            std::string fshPath = path + "Shader/DefaultShader.fsh";
            std::string gshPth = "";
            std::string textureDirPath = path + "Texture/Planets/";

            for (int i = 0; i < 10; i++)
            {
                // Entity name:
                ECSManager::Instance()->set_entityName(solarSystemIDs[i], planetNames[i]);

                // Transform:
                TransformComponent *transform = ECSManager::Instance()->get_component
                        <TransformComponent>(solarSystemIDs[i], COMPONENT_TRANSFORM);
                transform->scale = planetScales[i];
                transform->position = planetPositions[i];
                transform->rotation = planetRotation;

                // Mesh:
                MeshComponent mesh;
                mesh.path = path + "DefaultObjects/sphere20x20.obj";
                ECSManager::Instance()->create_component<MeshComponent>(solarSystemIDs[i],
                                                                        COMPONENT_MESH, mesh);

                // Material:
                MaterialComponent material;
                std::string texturePath = textureDirPath + planetNames[i] + ".jpg";
                material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
                ECSManager::Instance()->create_component<MaterialComponent>(solarSystemIDs[i],
                                                                    COMPONENT_MATERIAL, material);

                // Don't add orbit to the Sun:
                if (i == 0) { continue; }

                // Orbit:
                ECSManager::Instance()->create_component<OrbitComponent>(solarSystemIDs[i],
                                                                        COMPONENT_ORBIT);
                OrbitComponent *orbit = ECSManager::Instance()->get_component<OrbitComponent>
                                                        (solarSystemIDs[i], COMPONENT_ORBIT);

                orbit->mActive = true;
                if (i == 9) { orbit->mPrimaryEntityId = solarSystemIDs[3]; }
                else { orbit->mPrimaryEntityId = solarSystemIDs[0]; }
                orbit->mOrbitPeriod = orbitPeriods[i];

                OrbitSystem::Instance()->set_orbit_primary(solarSystemIDs[i], orbit->mPrimaryEntityId);
                OrbitSystem::Instance()->initialise_orbit(solarSystemIDs[i]);

                // Health:
                HealthComponent health { 100.0, 100.0 };
                ECSManager::Instance()->create_component(solarSystemIDs[i], COMPONENT_HEALTH, health);

                // Weapon:
                WeaponComponent weapon { 10.0, 1.5, 0.25, 0.0 };
                ECSManager::Instance()->create_component(solarSystemIDs[i], COMPONENT_WEAPON, weapon);
            }
        }
        QDEBUG("Solar system constructed.");
    }

    void entity_stress_test()
    {
        QDEBUG("Setting up Entity stress test.");

        // Test how many entities there are:
        unsigned int numEntities = ECSManager::Instance()->get_num_entities();
        QDEBUG("---> Start number of Entities: {0}", numEntities);

        // Work out how many entities need creating:
        numEntities = MAX_ENTITIES - numEntities;

        // Leave some room for projectiles:
        numEntities = numEntities / 2;

        // If there are less than four Entities left to create, there's no point:
        if (numEntities < 4) {
            QDEBUG("Entity number already near max. Stopping stress test.");
            return;
        }

        // Split into four groups:
        unsigned int group1 = (unsigned int)(numEntities / 4);
        unsigned int group2 = group1;
        unsigned int group3 = group1;
        unsigned int group4 = numEntities - group1 - group2 - group3;
        QDEBUG("---> Group 1: {0} Entities.", group1);
        QDEBUG("---> Group 2: {0} Entities.", group2);
        QDEBUG("---> Group 3: {0} Entities.", group3);
        QDEBUG("---> Group 4: {0} Entities.", group4);

        // Set group spawn points:
        glm::vec3 group1Spawn = glm::vec3(-15.0, 0.0, -25.0);
        glm::vec3 group2Spawn = glm::vec3( 15.0, 0.0, -25.0);
        glm::vec3 group3Spawn = glm::vec3( 15.0, 0.0,  25.0);
        glm::vec3 group4Spawn = glm::vec3(-15.0, 0.0,  25.0);

        // Set up the spawners:
        unsigned int group1ID = ECSManager::Instance()->create_entity();
        unsigned int group2ID = ECSManager::Instance()->create_entity();
        unsigned int group3ID = ECSManager::Instance()->create_entity();
        unsigned int group4ID = ECSManager::Instance()->create_entity();
/*
        EnemySpawnComponent enemySpawn1 { 0.1, 0.0, group1, 0 };
        EnemySpawnComponent enemySpawn2 { 0.1, 0.0, group2, 0 };
        EnemySpawnComponent enemySpawn3 { 0.1, 0.0, group3, 0 };
        EnemySpawnComponent enemySpawn4 { 0.1, 0.0, group4, 0 };
*/
        EnemySpawnComponent enemySpawn1 { 0.1, 0.0, 10, 0 };
        EnemySpawnComponent enemySpawn2 { 0.1, 0.0, 10, 0 };
        EnemySpawnComponent enemySpawn3 { 0.1, 0.0, 10, 0 };
        EnemySpawnComponent enemySpawn4 { 0.1, 0.0, 10, 0 };

        ECSManager::Instance()->create_component(group1ID, COMPONENT_ENEMY_SPAWNER, enemySpawn1);
        ECSManager::Instance()->create_component(group2ID, COMPONENT_ENEMY_SPAWNER, enemySpawn2);
        ECSManager::Instance()->create_component(group3ID, COMPONENT_ENEMY_SPAWNER, enemySpawn3);
        ECSManager::Instance()->create_component(group4ID, COMPONENT_ENEMY_SPAWNER, enemySpawn4);

        TransformComponent *transform1 = ECSManager::Instance()->get_component<TransformComponent>
                                                                (group1ID, COMPONENT_TRANSFORM);
        TransformComponent *transform2 = ECSManager::Instance()->get_component<TransformComponent>
                                                                (group2ID, COMPONENT_TRANSFORM);
        TransformComponent *transform3 = ECSManager::Instance()->get_component<TransformComponent>
                                                                (group3ID, COMPONENT_TRANSFORM);
        TransformComponent *transform4 = ECSManager::Instance()->get_component<TransformComponent>
                                                                (group4ID, COMPONENT_TRANSFORM);

        transform1->position = group1Spawn;
        transform2->position = group2Spawn;
        transform3->position = group3Spawn;
        transform4->position = group4Spawn;

        QDEBUG("Finished setting up Entity stress test.");
    }
}