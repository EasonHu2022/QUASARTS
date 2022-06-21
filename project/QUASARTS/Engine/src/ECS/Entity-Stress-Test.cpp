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
            0.f,    // Sun (no orbit, just here for indexing purposes).
            10.f,   // Mercury.
            12.5f,  // Venus.
            15.f,   // Earth.
            17.5f,  // Mars.
            25.f,   // Jupiter.
            35.f,   // Saturn.
            50.f,   // Uranus.
            70.f,   // Neptune.
            7.5f    // Moon.
        };

        // Health of each planet. Smaller planets are harder to hit so balance:
        std::vector<float> planetHealth = {
            1500.f,     // Sun.
            250.f,      // Mercury.
            300.f,      // Venus.
            350.f,      // Earth.
            350.f,      // Mars.
            750.f,      // Jupiter.
            600.f,      // Saturn.
            500.f,      // Uranus.
            500.f,      // Neptune.
            150.f       // Moon.
        };

        // Damage of each planet:
        std::vector<float> planetDamage = {
            0.f,    // Sun. Placeholder.
            7.5f,   // Mercury.
            10.f,   // Venus.
            12.5f,  // Earth.
            12.5f,  // Mars.
            20.f,   // Jupiter.
            15.f,   // Saturn.
            12.5f,  // Uranus.
            12.5f,  // Neptune.
            5.f     // Moon.
        };

        // Get internal assets path:
        auto path = FileModule::Instance()->get_internal_assets_path();

        // Add a light:
        QDEBUG("---> Adding light.");
        {
            unsigned int lightID = ECSManager::Instance()->create_entity();
            ECSManager::Instance()->set_entityName(lightID, "Light");
            LightComponent light;
            light.ambient = glm::vec3(1.0, 1.0, 1.0);
            light.diffuse = glm::vec3(1.0, 1.0, 1.0);
            light.specular = glm::vec3(0.0, 0.0, 0.0);
            ECSManager::Instance()->create_component(lightID, COMPONENT_LIGHTING, light);

            TransformComponent *lightTransform = ECSManager::Instance()->get_component
                                            <TransformComponent>(lightID, COMPONENT_TRANSFORM);
            lightTransform->position = planetPositions[0];
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

                // Add emissive light to the planets:
                if (i == 0) { material.material->emission = glm::vec3(0.15, 0.1, 0.0); }        // Sun.
                else if (i == 1) { material.material->emission = glm::vec3(0.1, 0.1, 0.1); }    // Mercury.
                else if (i == 2) { material.material->emission = glm::vec3(0.2, 0.1, 0.0); }    // Venus.
                else if (i == 3) { material.material->emission = glm::vec3(0.1, 0.1, 0.2); }    // Earth.
                else if (i == 4) { material.material->emission = glm::vec3(0.1, 0.0, 0.0); }    // Mars.
                else if (i == 5) { material.material->emission = glm::vec3(0.1, 0.1, 0.1); }    // Jupiter.
                else if (i == 6) { material.material->emission = glm::vec3(0.1, 0.1, 0.1); }    // Saturn.
                else if (i == 7) { material.material->emission = glm::vec3(0.0, 0.1, 0.1); }    // Uranus.
                else if (i == 8) { material.material->emission = glm::vec3(0.0, 0.0, 0.1); }    // Neptune.
                else if (i == 9) { material.material->emission = glm::vec3(0.1, 0.1, 0.1); }    // Moon.

                ECSManager::Instance()->create_component<MaterialComponent>(solarSystemIDs[i],
                                                                    COMPONENT_MATERIAL, material);

                // Health:
                HealthComponent health { planetHealth[i], planetHealth[i] };
                ECSManager::Instance()->create_component(solarSystemIDs[i], COMPONENT_HEALTH, health);

                // Don't add orbit or weapon to the Sun:
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

                // Weapon:
                WeaponComponent weapon { planetDamage[i], 7.5, 0.25, 0.0 };
                ECSManager::Instance()->create_component(solarSystemIDs[i], COMPONENT_WEAPON, weapon);
            }
        }

        // Add particle emitter to the sun:
        {
            ParticleComponent particle;
            particle.randomRotation = false;
            particle.is_on = true;
            particle.cone = false;
            particle.pps = 150.f;
            particle.gravity = 0.f;
            particle.averageSpeed = 1.3f;
            particle.averageLifeLength = 2.f;
            particle.averageScale = 3.f;
            particle.speedError = 0.f;
            particle.lifeError = 0.2f;
            particle.scaleError = 0.01f;
            particle.posError = 0.f;
            particle.direction = glm::vec3(0.f, 1.f, 0.f);
            particle.directionDeviation = 0.5f;
            particle.texture.rows = 8;

            ECSManager::Instance()->create_component(solarSystemIDs[0], COMPONENT_PARTICLE, particle);
        }

        ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(solarSystemIDs[0], COMPONENT_PARTICLE);
        std::string particleTex = path + "Texture/fire.png";
        particle2->path = particleTex;
        particle2->loadtex(particleTex.c_str());

        // Add armour to the Sun:
        ArmourComponent armour{20.f};
        ECSManager::Instance()->create_component(solarSystemIDs[0], COMPONENT_ARMOUR, armour);

        QDEBUG("Solar system constructed.");
    }

    void entity_stress_test()
    {
        QDEBUG("Setting up Entity stress test.");

        // Test how many entities there are:
        int numEntities = ECSManager::Instance()->get_num_entities();
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
        int groups[] = {
            numEntities / 4,
            numEntities / 4,
            numEntities / 4,
            numEntities
        };
        groups[3] -= groups[2] + groups[1] + groups[0];

        QDEBUG("---> Group 1: {0} Entities.", groups[0]);
        QDEBUG("---> Group 2: {0} Entities.", groups[1]);
        QDEBUG("---> Group 3: {0} Entities.", groups[2]);
        QDEBUG("---> Group 4: {0} Entities.", groups[3]);

        // Set group spawn points:
        glm::vec3 groupSpawns[] = {
            glm::vec3( 20.f, 0.f, -30.f),
            glm::vec3( 20.f, 0.f,  10.f),
            glm::vec3(-20.f, 0.f, -30.f),
            glm::vec3(-20.f, 0.f,  10.f)
        };

        // Set spawner rotations:
        glm::vec3 spawnerRotations[] = {
            glm::vec3(0.f, -45.f, 0.f),
            glm::vec3(0.f,  45.f, 0.f),
            glm::vec3(0.f,  45.f, 0.f),
            glm::vec3(0.f, -45.f, 0.f)
        };

        // Set up the spawners:
        for (int i = 0; i < 4; i++)
        {
            // Create the spawner and name it:
            unsigned int spawnerID = ECSManager::Instance()->create_entity();
            ECSManager::Instance()->set_entityName(spawnerID, "Enemy Spawner");

            // Define up the spawner settings:
            //EnemySpawnComponent enemySpawner { 0.1, 0.0, groups[i], 0, true, false };
            EnemySpawnComponent enemySpawner { 0.1f, 0.f, 10, 0, true, false };
            ECSManager::Instance()->create_component(spawnerID, COMPONENT_ENEMY_SPAWNER, enemySpawner);

            // Define the spawner transform:
            TransformComponent *transform = ECSManager::Instance()->get_component<TransformComponent>
                                                                    (spawnerID, COMPONENT_TRANSFORM);
            transform->position = groupSpawns[i];
            transform->rotation = spawnerRotations[i];
            transform->scale = glm::vec3(0.5f, 0.5f, 1.f);

            // Add a mesh:
            auto path = FileModule::Instance()->get_internal_assets_path();
            MeshComponent mesh;
            mesh.path = path + "Meshes/Portal.obj";
            ECSManager::Instance()->create_component(spawnerID, COMPONENT_MESH, mesh);

            // Add a material:
            MaterialComponent material;
            std::string texturePath = path + "Texture/Portal.png";
            std::string vshPath = path + "Shader/DefaultShader.vsh";
            std::string fshPath = path + "Shader/DefaultShader.fsh";
            std::string gshPath = "";
            material.material = new Material(vshPath, fshPath, gshPath, texturePath);
            material.material->emission = glm::vec3(20.f / 255.f, 0.f, 20.f / 255.f);
            ECSManager::Instance()->create_component(spawnerID, COMPONENT_MATERIAL, material);

            // Add a particle emitter:
            {
                ParticleComponent particle;
                particle.randomRotation = false;
                particle.is_on = true;
                particle.cone = false;
                particle.pps = 90.f;
                particle.gravity = 0.f;
                particle.averageSpeed = 2.f;
                particle.averageLifeLength = 2.f;
                particle.averageScale = 0.1f;
                particle.speedError = 0.3f;
                particle.lifeError = 0.2f;
                particle.scaleError = 0.01f;
                particle.posError = 0.f;
                particle.direction = glm::vec3(0.f, 1.f, 0.f);
                particle.directionDeviation = 0.5f;
                particle.texture.rows = 4;

                ECSManager::Instance()->create_component(spawnerID, COMPONENT_PARTICLE, particle);
            }

            ParticleComponent *particle2 = ECSManager::Instance()->get_component
                            <ParticleComponent>(spawnerID, COMPONENT_PARTICLE);

            std::string particleTex = path + "Texture/cosmic.png";
            particle2->path = particleTex;
            particle2->loadtex(particleTex.c_str());
        }

        QDEBUG("Finished setting up Entity stress test.");
    }
}