#include "Showcase-Video.h"

namespace Engine
{
    // Singleton:
	ShowcaseVideo *ShowcaseVideo::instance = nullptr;

	// Create the instance of the ECSManager:
	ShowcaseVideo *ShowcaseVideo::Instance()
    {
		if (nullptr == instance) {
			instance = new ShowcaseVideo();
        }
		return instance;
	}

    void ShowcaseVideo::update()
    {
        if (video_intro_running) { update_camera_intro(); }
        if (script_test_running) { update_script_test(); }
        if (level_demo_running) { update_level_demo(); }
    }

    void ShowcaseVideo::create_video_intro()
    {
        QDEBUG("Beginning showcase: Intro");
        construct_solar_system();
        video_intro_running = true;

        script_test_running = false;
        level_demo_running = false;

        unsigned int cameraID = ECSManager::Instance()->get_camera();
        TransformComponent *cameraTransform = ECSManager::Instance()->get_component
                                        <TransformComponent>(cameraID, COMPONENT_TRANSFORM);
        cameraTransform->position = glm::vec3(0.f, 13.f, 6.f);
        cameraTransform->rotation = glm::vec3(-40.0, -90.0, 0.0);
    }

    void ShowcaseVideo::create_quasarts_logo()
    {
        // Create the Quasarts Logo:
        QDEBUG("---> Creating Quasarts Logo");
        {
            unsigned int logoID = ECSManager::Instance()->create_entity();

            auto path = FileModule::Instance()->get_internal_assets_path();
            std::string vshPath = path + "Shader/DefaultShader.vsh";
            std::string fshPath = path + "Shader/DefaultShader.fsh";
            std::string gshPth = "";
            std::string texturePath = path + "Texture/Quasarts-Logo.png";

            // Entity name:
            ECSManager::Instance()->set_entityName(logoID, "Quasarts Logo");

            // Transform:
            TransformComponent *transform = ECSManager::Instance()->get_component
                    <TransformComponent>(logoID, COMPONENT_TRANSFORM);
            transform->scale = glm::vec3(0.8f, 0.8f, 0.8f);
            transform->position = glm::vec3(-5.f, 0.f, -15.f);
            transform->rotation = glm::vec3(0.f, 0.f, 0.f);

            // Mesh:
            MeshComponent mesh;
            mesh.path = path + "Meshes/Quasarts-Logo.obj";
            ECSManager::Instance()->create_component<MeshComponent>(logoID, COMPONENT_MESH, mesh);

            // Material:
            MaterialComponent material;
            material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);

            // Add emissive light to the logo:
            material.material->emission = glm::vec3(0.1f, 0.1f, 0.1f);

            ECSManager::Instance()->create_component<MaterialComponent>(logoID, COMPONENT_MATERIAL, material);
        }
    }

    void ShowcaseVideo::update_camera_intro()
    {
        TransformComponent *transform = ECSManager::Instance()->get_component<TransformComponent>
                                    (ECSManager::Instance()->get_camera(), COMPONENT_TRANSFORM);

        glm::vec3 sunPosition = glm::vec3(0.f, 0.f, -10.f);

        // Get time since last frame:
        auto deltaT = TimeModule::Instance()->get_frame_delta_time().sec();
        static float timeSpent = -25.f;
        float totalTime = 7.5f;

        // Major events:
        static bool startMoving = false;
        static bool stopDecreasingRadius = false;
        static bool stopIncreasingRadius = true;
        static bool stopXRotation = false;
        static bool stopYDecrease = false;
        static bool stopCircularMotion = false;
        static bool stopCameraRotation = false;
        static int rotationState = 0;

        if (startMoving && stopDecreasingRadius && stopIncreasingRadius && stopXRotation
                        && stopYDecrease && stopCircularMotion && stopCameraRotation)
        {
            QDEBUG("Ending showcase: Intro");
            video_intro_running = false;
            return;
        }

        if ((timeSpent >= 0.f) && (startMoving == false))
        {
            startMoving = true;
        }

        if (startMoving == false)
        {
            timeSpent += deltaT;
            return;
        }

        // Zoom in:
        if (stopDecreasingRadius == false)
        {
            float distanceToTravel = 9.f;

            // Get xy vector between camera and sun:
            glm::vec3 moveVector = sunPosition - transform->position;
            moveVector.y = 0.f;
            float distanceToSun = glm::length(moveVector);
            moveVector = glm::normalize(moveVector);

            transform->position += moveVector * float(deltaT) * (distanceToTravel / (totalTime * (3.f / 5.f)));

            moveVector = sunPosition - transform->position;
            moveVector.y = 0.f;
            distanceToSun = glm::length(moveVector);
            if (distanceToSun < 7.f)
            {
                stopDecreasingRadius = true;
                stopIncreasingRadius = false;
            }
        }

        if(stopIncreasingRadius == false)
        {
            float distanceToTravel = 2.f;

            // Get xy vector between camera and sun:
            glm::vec3 moveVector = transform->position - sunPosition;
            moveVector.y = 0.f;
            float distanceToSun = glm::length(moveVector);
            moveVector = glm::normalize(moveVector);

            transform->position += moveVector * float(deltaT) * (distanceToTravel / (totalTime * (2.f / 5.f)));

            moveVector = transform->position - sunPosition;
            moveVector.y = 0.f;
            distanceToSun = glm::length(moveVector);
            if (distanceToSun > 9.f)
            {
                stopIncreasingRadius = true;
            }
        }

        // Rotate camera up to level:
        if (stopXRotation == false)
        {
            float distanceToTravel = 40.f;

            transform->rotation.x += deltaT * (distanceToTravel / (totalTime * (2.f / 5.f)));
            if (transform->rotation.x > 0.f) {
                stopXRotation = true;
            }
        }

        // Decrease Y position to be level with Sun:
        if (stopYDecrease == false)
        {
            float distanceToTravel = 13.f;

            transform->position.y -= deltaT * (distanceToTravel / (totalTime * (2.f / 5.f)));

            if (transform->position.y < 0.f) {
                stopYDecrease = true;
            }
        }

        // Move around the Sun:
        if (stopCircularMotion == false)
        {
            float distanceToTravel = 412.5f;

            float angularVelocity = distanceToTravel / totalTime;
            float angularVelocityRadians = glm::radians(angularVelocity);

            glm::vec3 currentPos = transform->position - sunPosition;

            transform->position.x = (currentPos.x * glm::cos(angularVelocityRadians * deltaT))
                - (currentPos.z * glm::sin(angularVelocityRadians * deltaT)) + sunPosition.x;
            
            transform->position.z = (currentPos.z * glm::cos(angularVelocityRadians * deltaT))
                + (currentPos.x * glm::sin(angularVelocityRadians * deltaT)) + sunPosition.z;

            if (transform->position.z < -5.f)
            {
                if (rotationState == 0)
                {
                    rotationState = 1;
                }
                else if (rotationState == 2)
                {
                    stopCircularMotion = true;
                }
            }
            else
            {
                if (rotationState == 1) {
                    create_quasarts_logo();
                    rotationState = 2;
                }
            }
        }

        // Rotate the camera:
        if (stopCameraRotation == false)
        {
            float distanceToTravel = 380.f;

            float angularVelocity = distanceToTravel / totalTime;
            transform->rotation.y += angularVelocity * deltaT;
            if (transform->rotation.y > 290.f) {
                stopCameraRotation = true;
            }
        }
    }

    void ShowcaseVideo::create_script_test(int option)
    {
        QDEBUG("Beginning showcase: Scripting");

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
            lightTransform->position = glm::vec3(0.f, 5.f, 0.f);
        }

        QDEBUG("---> Transforming camera.");
        // Transform the camera:
        {
            unsigned int cameraID = ECSManager::Instance()->get_camera();
            TransformComponent *cameraTransform = ECSManager::Instance()->get_component
                                            <TransformComponent>(cameraID, COMPONENT_TRANSFORM);
            if (option == 0)
            {
                cameraTransform->position = glm::vec3(25.f, 0.f, 0.f);
                cameraTransform->rotation = glm::vec3(0.f, 180.f, 0.f);
            }
            else
            {
                cameraTransform->position = glm::vec3(1.5f, 0.f, -16.75f);
                cameraTransform->rotation = glm::vec3(0.f, 120.f, 0.f);
            }
        }

        // Create planets:
        QDEBUG("---> Creating planets.");

        moonID = Engine::ECSManager::Instance()->create_entity();
        earthID = Engine::ECSManager::Instance()->create_entity();

        auto path = FileModule::Instance()->get_internal_assets_path();
        std::string vshPath = path + "Shader/DefaultShader.vsh";
        std::string fshPath = path + "Shader/DefaultShader.fsh";
        std::string gshPth = "";

        unsigned int entityIDs[] = { moonID, earthID };
        for (int i = 0; i < 2; i++)
        {
            TransformComponent *transform = ECSManager::Instance()->get_component<TransformComponent>(entityIDs[i], COMPONENT_TRANSFORM);
            if (i == 0)
            {
                transform->position = glm::vec3(0.f, 0.f, 15.f);
                transform->rotation = glm::vec3(-90.f, 0.f, 0.f);
            }
            else
            {
                transform->position = glm::vec3(0.f, 0.f, -15.f);
                transform->rotation = glm::vec3(-90.f, 0.f, 0.f);
            }

            MeshComponent mesh;
            mesh.path = path + "DefaultObjects/sphere20x20.obj";
            ECSManager::Instance()->create_component(entityIDs[i], COMPONENT_MESH, mesh);

            MaterialComponent material;
            std::string texturePath;
            if (i == 0)
            {
                texturePath = path + "Texture/Planets/Moon.jpg";
            }
            else
            {
                texturePath = path + "Texture/Planets/Earth.jpg";
            }
            material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
            ECSManager::Instance()->create_component(entityIDs[i], COMPONENT_MATERIAL, material);

            {
                ParticleComponent particle;
                particle.randomRotation = false;
                particle.is_on = false;
                particle.cone = false;
                particle.pps = 90.f;
                particle.gravity = 0.f;
                particle.averageSpeed = 1.f;
                particle.averageLifeLength = 2.f;
                particle.averageScale = 0.75f;
                particle.speedError = 0.3f;
                particle.lifeError = 0.2f;
                particle.scaleError = 0.01f;
                particle.posError = 0.f;
                particle.direction = glm::vec3(0.f, 1.f, 0.f);
                particle.directionDeviation = 0.5f;
                particle.texture.rows = 4;

                ECSManager::Instance()->create_component(entityIDs[i], COMPONENT_PARTICLE, particle);
            }

            ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(entityIDs[i], COMPONENT_PARTICLE);
            std::string particleTex = path + "Texture/particleAtlas.png";
            particle2->path = particleTex;
            particle2->loadtex(particleTex.c_str());

            ECSManager::Instance()->create_component<CollisionSphereComponent>(entityIDs[i], COMPONENT_COLLISION_SPHERE);
            Engine::CollisionSystem::Instance()->init_collision_component(entityIDs[i], COMPONENT_COLLISION_SPHERE);

            HealthComponent health;
            health.max_health = 100.f;
            health.current_health = 100.f;
            ECSManager::Instance()->create_component(entityIDs[i], COMPONENT_HEALTH, health);

            // Only the Moon needs a script:
            if (i == 0)
            {
                ECSManager::Instance()->create_component<ScriptComponent>(entityIDs[i], COMPONENT_SCRIPT);
                ScriptComponent *script = ECSManager::Instance()->get_component<ScriptComponent>(entityIDs[i], COMPONENT_SCRIPT);

                std::string script_file = path + "Scripts/ShowcaseVideo.lua";
                ScriptSystem::Instance()->initComponent(script, script_file, entityIDs[i]);
                ScriptSystem::Instance()->loadScript(script);
            }
        }

        QDEBUG("Script test created.");
        script_test_running = true;

        video_intro_running = false;
        level_demo_running = false;
    }

    void ShowcaseVideo::update_script_test()
    {
        static bool moonDestroyed = false;
        static bool earthDestroyed = false;

        static bool moonParticleRemoved = false;
        static bool earthParticleRemoved = false;

        static float moonCountdown = 0.f;
        static float earthCountdown = 0.f;
        float maxCountdown = 2.f;

        auto deltaT = TimeModule::Instance()->get_frame_delta_time().sec();

        if (moonDestroyed)
        {
            moonCountdown += deltaT;
        }
        if (earthDestroyed)
        {
            earthCountdown += deltaT;
        }

        TransformComponent *moonTransform = ECSManager::Instance()->get_component<TransformComponent>(moonID, COMPONENT_TRANSFORM);
        HealthComponent *healthMoon = ECSManager::Instance()->get_component<HealthComponent>(moonID, COMPONENT_HEALTH);
        HealthComponent *healthEarth = ECSManager::Instance()->get_component<HealthComponent>(earthID, COMPONENT_HEALTH);

        if ((healthMoon->current_health <= 0.f) && !moonDestroyed)
        {
            destroy_sphere(moonID);
            moonDestroyed = true;
        }
        if ((healthEarth->current_health <= 0.f) && !earthDestroyed)
        {
            destroy_sphere(earthID);
            earthDestroyed = true;
        }

        if ((moonCountdown >= maxCountdown) && !moonParticleRemoved)
        {
            ECSManager::Instance()->destroy_component<ParticleComponent>(moonID, COMPONENT_PARTICLE);
            moonParticleRemoved = true;
        }

        if ((earthCountdown >= maxCountdown) && !earthParticleRemoved)
        {
            ECSManager::Instance()->destroy_component<ParticleComponent>(earthID, COMPONENT_PARTICLE);
            earthParticleRemoved = true;
        }

        if ((moonCountdown >= maxCountdown) && (earthCountdown >= maxCountdown))
        {
            QDEBUG("Ending showcase: Scripting");
            script_test_running = false;
        }
    }

    void ShowcaseVideo::destroy_sphere(unsigned int entityID)
    {
        quasarts_component_mask mask{};
        mask.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        mask.mask += (uint64_t)1 << COMPONENT_MESH;
        mask.mask += (uint64_t)1 << COMPONENT_MATERIAL;
        mask.mask += (uint64_t)1 << COMPONENT_PARTICLE;
        mask.mask += (uint64_t)1 << COMPONENT_COLLISION_SPHERE;

        Entity* entity = ECSManager::Instance()->get_entity(entityID);
        quasarts_component_mask *entityMask = entity->get_componentMask();
        if ((mask.mask & entityMask->mask) != mask.mask) { return; }

        // Remove mesh:
        ECSManager::Instance()->destroy_component<MeshComponent>(entityID, COMPONENT_MESH);
        ECSManager::Instance()->destroy_component<MaterialComponent>(entityID, COMPONENT_MATERIAL);

        // Turn particle emitter on:
        ParticleComponent *particle = ECSManager::Instance()->get_component<ParticleComponent>(entityID, COMPONENT_PARTICLE);
        particle->is_on = true;
    }

    void ShowcaseVideo::give_player_weapon()
    {
        QDEBUG("The player now has a weapon! F11 for free mouse movement.");

        // Add a weapon Component to the Player (camera):
        unsigned int playerID = ECSManager::Instance()->get_camera();

        WeaponComponent weapon;
        weapon.base_damage = 20.f;
        weapon.attack_range = 0.f;
        weapon.attack_speed = 0.15f;
        weapon.lastFire = 0.f;
        ECSManager::Instance()->create_component(playerID, COMPONENT_WEAPON, weapon);

        // Add the appopriate UI elements, priority 1 / 5:
        auto path = FileModule::Instance()->get_internal_assets_path();

        // Crosshair:
        unsigned int crosshairID = ECSManager::Instance()->create_entity();
        ECSManager::Instance()->set_entityName(crosshairID, "Crosshair");

        {
            UIElementComponent uiElementCrosshair;
            uiElementCrosshair.uiTexPath = path + "Texture/UI/Crosshair.png";
            uiElementCrosshair.uiTransparency = true;
            uiElementCrosshair.priority = 1;
            ECSManager::Instance()->create_component(crosshairID, COMPONENT_UI_ELEMENT, uiElementCrosshair);
        }

        UIElementComponent *uiElementCrosshair = ECSManager::Instance()->get_component<UIElementComponent>
                                                                        (crosshairID, COMPONENT_UI_ELEMENT);
        uiElementCrosshair->uiElementTexture = new Texture2D(uiElementCrosshair->uiTexPath);

        // Player weapons:
        unsigned int playerGunsID = ECSManager::Instance()->create_entity();
        ECSManager::Instance()->set_entityName(playerGunsID, "Player Weapons");

        {
            UIElementComponent uiElementPlayerGuns;
            uiElementPlayerGuns.uiTexPath = path + "Texture/UI/PlayerGun.png";
            uiElementPlayerGuns.uiTransparency = false;
            uiElementPlayerGuns.priority = 1;
            ECSManager::Instance()->create_component(playerGunsID, COMPONENT_UI_ELEMENT, uiElementPlayerGuns);
        }

        UIElementComponent *uiElementPlayerGuns = ECSManager::Instance()->get_component<UIElementComponent>
                                                                        (playerGunsID, COMPONENT_UI_ELEMENT);
        uiElementPlayerGuns->uiElementTexture = new Texture2D(uiElementPlayerGuns->uiTexPath);
    }

    void ShowcaseVideo::start_level_demo()
    {
        QDEBUG("Beginning level");
        level_demo_running = true;

        video_intro_running = false;
        script_test_running = false;
    }

    void ShowcaseVideo::update_level_demo()
    {
        static float timeElapsed = 0.f;
        float firstSpawner = 10.f;
        float secondSpawner = 40.f;
        float thirdSpawner = 80.f;
        float fourthSpawner = 120.f;
        float gameEnd = 135.f;
        static int spawnersSpawned = 0;

        // Set spawner spawn points:
        glm::vec3 spawnerSpawns[] = {
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

        auto deltaT = TimeModule::Instance()->get_frame_delta_time().sec();
        timeElapsed += deltaT;

        if ((timeElapsed > firstSpawner) && (spawnersSpawned == 0))
        {
            spawn_enemy_spawner(spawnerSpawns[0], spawnerRotations[0], 50);
            spawnersSpawned = 1;
            std::string uiTexPath = "Texture/UI/Level-1/Portal-1.png";
            create_UI_element(uiTexPath, false, 1, 10.f);
            AudioSystem::Instance()->playSoundClip("message1");
        }
        else if ((timeElapsed > secondSpawner) && (spawnersSpawned == 1))
        {
            spawn_enemy_spawner(spawnerSpawns[1], spawnerRotations[1], 75);
            spawnersSpawned = 2;
            std::string uiTexPath = "Texture/UI/Level-1/Portal-2.png";
            create_UI_element(uiTexPath, false, 1, 10.f);
            AudioSystem::Instance()->playSoundClip("message1");
        }
        else if ((timeElapsed > thirdSpawner) && (spawnersSpawned == 2))
        {
            spawn_enemy_spawner(spawnerSpawns[2], spawnerRotations[2], 150);
            spawnersSpawned = 3;
            std::string uiTexPath = "Texture/UI/Level-1/Portal-3.png";
            create_UI_element(uiTexPath, false, 1, 10.f);
            AudioSystem::Instance()->playSoundClip("message1");
        }
        else if ((timeElapsed > fourthSpawner) && (spawnersSpawned == 3))
        {
            spawn_enemy_spawner(spawnerSpawns[3], spawnerRotations[3], 225);
            spawnersSpawned = 4;
            std::string uiTexPath = "Texture/UI/Level-1/Portal-4.png";
            create_UI_element(uiTexPath, false, 1, 10.f);
            AudioSystem::Instance()->playSoundClip("message1");
        }
        else if ((timeElapsed > gameEnd) && (spawnersSpawned == 4))
        {
            static int planetCountStart = count_planets();

            quasarts_entity_ID_mask *enemyMask = CombatSystem::Instance()->get_entity_ID_mask(0);
            quasarts_entity_ID_mask *enemySpawnMask = CombatSystem::Instance()->get_entity_ID_mask(1);
            quasarts_entity_ID_mask *sunMask = CombatSystem::Instance()->get_entity_ID_mask(6);

            ECSManager *active_manager = CombatSystem::Instance()->get_manager();
            EnemySpawnComponent *enemySpawner;

            bool sunPresent = false;
            std::string messageTex;

            // Check for win/lose condition:
            for (int i = 0; i < MAX_ENTITIES; i++)
            {
                if (enemyMask->mask[i] == 1)
                {
                    // If an enemy lives, the game isn't over:
                    return;
                }
                if (enemySpawnMask->mask[i] == 1)
                {
                    // If a spawner still has enemies to spawn, the game isn't over:
                    enemySpawner = active_manager->get_component<EnemySpawnComponent>(i, COMPONENT_ENEMY_SPAWNER);
                    if (enemySpawner->currentSpawns != enemySpawner->maxSpawns) { return; }
                }
                if (sunMask->mask[i] == 1)
                {
                    sunPresent = true;
                }
            }

            if (!sunPresent)
            {
                // We lost:
                messageTex = "Texture/UI/Level-1/Failure.png";
            }
            else
            {
                // We won:
                int planetCountEnd = count_planets();
                if (planetCountEnd == planetCountStart) { messageTex = "Texture/UI/Level-1/Success-No-Losses.png"; }
                else { messageTex = "Texture/UI/Level-1/Success-Losses.png"; }
            }

            create_UI_element(messageTex, false, 1, 10.f);
            AudioSystem::Instance()->playSoundClip("message1");
            level_demo_running = false;
            timeElapsed = 0.f;
        }
    }

    void ShowcaseVideo::spawn_enemy_spawner(glm::vec3 position, glm::vec3 rotation, int maxSpawns)
    {
        // Create the spawner and name it:
        unsigned int spawnerID = ECSManager::Instance()->create_entity();
        ECSManager::Instance()->set_entityName(spawnerID, "Enemy Spawner");

        // Define up the spawner settings:
        EnemySpawnComponent enemySpawner { 0.25f, 0.f, maxSpawns, 0, true, true };
        ECSManager::Instance()->create_component(spawnerID, COMPONENT_ENEMY_SPAWNER, enemySpawner);

        // Define the spawner transform:
        TransformComponent *transform = ECSManager::Instance()->get_component<TransformComponent>
                                                                (spawnerID, COMPONENT_TRANSFORM);
        transform->position = position;
        transform->rotation = rotation;
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

    void ShowcaseVideo::create_UI_element(std::string uiTexPath, bool uiTransparency, int priority, float totalLifetime)
    {
        auto path = FileModule::Instance()->get_internal_assets_path();
        unsigned int uiElementID = ECSManager::Instance()->create_entity();
        ECSManager::Instance()->set_entityName(uiElementID, "Message");

        {
            UIElementComponent uiElement;
            uiElement.uiTexPath = path + uiTexPath;
            uiElement.uiTransparency = uiTransparency;
            uiElement.priority = priority;
            ECSManager::Instance()->create_component(uiElementID, COMPONENT_UI_ELEMENT, uiElement);
        }

        UIElementComponent *uiElement = ECSManager::Instance()->get_component<UIElementComponent>
                                                            (uiElementID, COMPONENT_UI_ELEMENT);
        uiElement->uiElementTexture = new Texture2D(uiElement->uiTexPath);

        if (totalLifetime < 0.f) { return; }

        LifeSpanComponent life { 0.f, totalLifetime };
        ECSManager::Instance()->create_component(uiElementID, COMPONENT_LIFESPAN, life);
    }

    int ShowcaseVideo::count_planets()
    {
        quasarts_entity_ID_mask *planetMask = CombatSystem::Instance()->get_entity_ID_mask(2);
        ECSManager *active_manager = CombatSystem::Instance()->get_manager();

        int planetCount = 0;

        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (planetMask->mask[i] == 1)
            {
                planetCount++;
            }
        }

        return planetCount;
    }
}