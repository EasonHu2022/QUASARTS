#include "CombatSystem.h"

namespace Engine
{
    // Singleton:
    CombatSystem* CombatSystem::instance = nullptr;

    // Create the instance of the CombatSystem:
    CombatSystem* CombatSystem::Instance() {
        if (nullptr == instance)
            instance = new CombatSystem();
        return instance;
    }

    CombatSystem::CombatSystem() {
        // Set the Component mask for enemies:
        quasarts_component_mask mask1;
        mask1.mask = 0;
        mask1.mask += (uint64_t)1 << COMPONENT_ENEMY;
        mask1.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        mask1.mask += (uint64_t)1 << COMPONENT_MESH;
        mask1.mask += (uint64_t)1 << COMPONENT_MATERIAL;
        mask1.mask += (uint64_t)1 << COMPONENT_HEALTH;
        mask1.mask += (uint64_t)1 << COMPONENT_WEAPON;
        mask1.mask += (uint64_t)1 << COMPONENT_PARTICLE;
        add_component_mask(mask1);

        // Set the Component mask for enemy spawners:
        quasarts_component_mask mask2;
        mask2.mask = 0;
        mask2.mask += (uint64_t)1 << COMPONENT_ENEMY_SPAWNER;
        mask2.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        add_component_mask(mask2);

        // Set a Component mask for Entities with orbit Components:
        quasarts_component_mask mask3;
        mask3.mask = 0;
        mask3.mask += (uint64_t)1 << COMPONENT_ORBIT;
        mask3.mask += (uint64_t)1 << COMPONENT_HEALTH;
        mask3.mask += (uint64_t)1 << COMPONENT_WEAPON;
        add_component_mask(mask3);

        // Set a Component mask for projectiles:
        quasarts_component_mask mask4;
        mask4.mask = 0;
        mask4.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        mask4.mask += (uint64_t)1 << COMPONENT_PROJECTILE;
        mask4.mask += (uint64_t)1 << COMPONENT_MESH;
        mask4.mask += (uint64_t)1 << COMPONENT_MATERIAL;
        add_component_mask(mask4);

        // Set a Component mask for short-lived things:
        quasarts_component_mask mask5;
        mask5.mask = 0;
        mask5.mask += (uint64_t)1 << COMPONENT_LIFESPAN;
        add_component_mask(mask5);

        // Set a Component mask for the Player:
        quasarts_component_mask mask6;
        mask6.mask = 0;
        mask6.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        mask6.mask += (uint64_t)1 << COMPONENT_CAMERA;
        mask6.mask += (uint64_t)1 << COMPONENT_WEAPON;
        add_component_mask(mask6);

        // Set a Component mask for the Sun:
        quasarts_component_mask mask7;
        mask7.mask = 0;
        mask7.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        mask7.mask += (uint64_t)1 << COMPONENT_MESH;
        mask7.mask += (uint64_t)1 << COMPONENT_MATERIAL;
        mask7.mask += (uint64_t)1 << COMPONENT_PARTICLE;
        mask7.mask += (uint64_t)1 << COMPONENT_HEALTH;
        mask7.mask += (uint64_t)1 << COMPONENT_ARMOUR;
        add_component_mask(mask7);

        // Seed random generator with current time:
        srand(time(nullptr));
    }

    void CombatSystem::update()
    {
        // Get manager, declare Component pointers and get masks:
        ECSManager* active_manager = get_manager();
        EnemyComponent *enemy;
        EnemySpawnComponent *enemySpawner;
        TransformComponent *transform;
        HealthComponent *health;
        WeaponComponent *weapon;
        ProjectileComponent *projectile;
        LifeSpanComponent *lifeSpan;
        ArmourComponent *armour;
        ParticleComponent *particle;

        quasarts_entity_ID_mask *enemies = get_entity_ID_mask(0);
        quasarts_entity_ID_mask *enemySpawners = get_entity_ID_mask(1);
        quasarts_entity_ID_mask *planetMask = get_entity_ID_mask(2);
        quasarts_entity_ID_mask *projectileMask = get_entity_ID_mask(3);
        quasarts_entity_ID_mask *lifeSpanMask = get_entity_ID_mask(4);
        quasarts_entity_ID_mask *playerMask = get_entity_ID_mask(5);
        quasarts_entity_ID_mask *sunMask = get_entity_ID_mask(6);

        // Check the time:
        auto deltaT = TimeModule::Instance()->get_frame_delta_time().sec();

        GLFWwindow *app_window = Application::Instance->get_glfw_context();
        static bool includeSun = false;

        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (enemies->mask[i] == 1)
            {
                // Get the Components:
                enemy = active_manager->get_component<EnemyComponent>(i, COMPONENT_ENEMY);
                enemySpawner = active_manager->get_component<EnemySpawnComponent>
                                        (enemy->spawnerID, COMPONENT_ENEMY_SPAWNER);
                transform = active_manager->get_component<TransformComponent>
                                                    (i, COMPONENT_TRANSFORM);
                health = active_manager->get_component<HealthComponent>(i, COMPONENT_HEALTH);
                weapon = active_manager->get_component<WeaponComponent>(i, COMPONENT_WEAPON);

                MaterialComponent *material = active_manager->get_component<MaterialComponent>
                                                                    (i, COMPONENT_MATERIAL);
                material->material->emission = glm::vec3(
                                    1.0 - (health->current_health / health->max_health),
                                    (1.0 - (health->current_health / health->max_health)) * 0.2,
                                    (1.0 - (health->current_health / health->max_health)) * 0.2
                );

                // Check health:
                if (health->current_health <= 0.0)
                {
                    //QDEBUG("Enemy {0} died.", i);
                    //QDEBUG("Number of entities present: {0}", active_manager->get_num_entities());
                    //QDEBUG("Framerate: {0}", 1.0 / deltaT);
                    unsigned int numEntities = active_manager->get_num_entities();
                    active_manager->destroy_entity(i);
                    AudioSystem::Instance()->playSoundClip("explosion4");
                    if (enemySpawner->oneShot == false)
                    {
                        enemySpawner->currentSpawns--;
                    }
                    continue;
                }

                // If the target planet is gone, choose a new one:
                if ((enemy->targetID != TOO_MANY_ENTITIES) && (planetMask->mask[enemy->targetID] == 0))
                {
                    enemy->targetID = TOO_MANY_ENTITIES;
                }

                // If the enemy has no target, choose one:
                if (enemy->targetID == TOO_MANY_ENTITIES)
                {
                    std::vector<unsigned int> planetIDs;
                    for (int j = 0; j < MAX_ENTITIES; j++)
                    {
                        if (planetMask->mask[j] == 1)
                        {
                            planetIDs.push_back(j);
                        }
                    }

                    // Now choose a planet at random:
                    if (planetIDs.size() > 0)
                    {
                        int randPlanet = rand() % planetIDs.size();
                        enemy->targetID = planetIDs[randPlanet];
                    }
                }

                // If still no target, see if there's a Sun to target:
                if (enemy->targetID == TOO_MANY_ENTITIES)
                {
                    for (int j = 0; j < MAX_ENTITIES; j++)
                    {
                        if (sunMask->mask[j] == 1)
                        {
                            enemy->targetID = j;

                            // Suns are now fair game - enable Sun health loss:
                            includeSun = true;
                            break;
                        }
                    }
                }

                // If STILL no target, skip:
                if (enemy->targetID == TOO_MANY_ENTITIES) { continue; }

                // Find the target position:
                TransformComponent *targetTransform = active_manager->get_component
                        <TransformComponent>(enemy->targetID, COMPONENT_TRANSFORM);

                // Move and rotate the enemy towards the target:
                glm::vec3 targetDirection = targetTransform->position - transform->position;
                float targetDistance = glm::length(targetDirection);
                float directionMult;
                if (targetDistance <= targetTransform->scale.x * 2.f + 1.f) { directionMult = -1.0; }
                else if (targetDistance <= targetTransform->scale.x * 3.f + 1.f) { directionMult = 0.0; }
                else { directionMult = 1.0; }

                transform->position += glm::normalize(targetDirection) * float(deltaT)
                                * float(directionMult) * float(enemy->moveSpeedMult);
                transform->rotation.y = glm::degrees(glm::acos(glm::dot(glm::normalize(targetDirection),
                                                                            glm::vec3(0.f, 0.f, 1.f))));
                if (targetDirection.x < 0.f) { transform->rotation.y *= -1.f; }

                particle = active_manager->get_component<ParticleComponent>(i, COMPONENT_PARTICLE);
                particle->direction = glm::normalize(targetDirection) * -1.f;

                // Handle "collision" with other enemies:
                for (int j = 0; j < MAX_ENTITIES; j++)
                {
                    if (j == i) { continue; }
                    if (enemies->mask[j] == 1)
                    {
                        // Find enemy position:
                        targetTransform = active_manager->get_component<TransformComponent>
                                                                    (j, COMPONENT_TRANSFORM);
                        targetDirection = targetTransform->position - transform->position;
                        float distanceToTarget = glm::length(targetDirection);
                        if (distanceToTarget < 0.5f)
                        {
                            distanceToTarget = 0.5f - distanceToTarget;
                            targetTransform->position += glm::normalize(targetDirection) * distanceToTarget;
                            transform->position -= glm::normalize(targetDirection) * distanceToTarget;
                        }
                    }
                }

                // Check the last time the enemy fired a projectile:
                weapon->lastFire += deltaT;
                if (weapon->lastFire < weapon->attack_speed) { continue; }
                if (targetDistance <= weapon->attack_range)
                {
                    // Fire a projectile at the target:
                    shoot(i, enemy->targetID, 0);
                    weapon->lastFire = 0.0;
                }
            }

            if (enemySpawners->mask[i] == 1)
            {
                // Get the Components:
                enemySpawner = active_manager->get_component<EnemySpawnComponent>
                                                    (i, COMPONENT_ENEMY_SPAWNER);
                transform = active_manager->get_component<TransformComponent>
                                                        (i, COMPONENT_TRANSFORM);

                if (enemySpawner->isActive == false) { continue; }

                // Update the spawner time:
                enemySpawner->lastSpawn += deltaT;

                // If enough time has accumulated, spawn a new enemy:
                if ((enemySpawner->lastSpawn >= enemySpawner->spawnRate)
                    && (enemySpawner->currentSpawns < enemySpawner->maxSpawns))
                {
                    spawn_enemy(enemySpawner, i, 5.0);
                }
            }

            if (planetMask->mask[i] == 1)
            {
                // Get the Components:
                weapon = active_manager->get_component<WeaponComponent>(i, COMPONENT_WEAPON);
                health = active_manager->get_component<HealthComponent>(i, COMPONENT_HEALTH);
                transform = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);

                // If health is at 0, kill the planet:
                if (health->current_health <= 0.0)
                {
                    kill_planet(i);
                    continue;
                }

                weapon->lastFire += deltaT;

                // See if we can shoot at the enemy (if not, skip):
                if (weapon->lastFire < weapon->attack_speed) { continue; }
                
                // Find any enemy targeting the planet:
                for (int j = 0; j < MAX_ENTITIES; j++)
                {
                    if (enemies->mask[j] == 1)
                    {
                        enemy = active_manager->get_component<EnemyComponent>(j, COMPONENT_ENEMY);
                        if (enemy->targetID == i)
                        {
                            TransformComponent *enemyTransform = active_manager->get_component
                                                <TransformComponent>(j, COMPONENT_TRANSFORM);
                            
                            float distance = glm::length(enemyTransform->position - transform->position);

                            if (distance <= weapon->attack_range)
                            {
                                // Fire a projectile at the enemy:
                                shoot(i, j, 1);
                                weapon->lastFire = 0.0;
                                break;
                            }
                        }
                    }
                }
            }

            if (projectileMask->mask[i] == 1)
            {
                projectile = active_manager->get_component<ProjectileComponent>(i, COMPONENT_PROJECTILE);
                projectile->currentTime += deltaT;

                // If the time is up, expire:
                if (projectile->currentTime >= projectile->maxTime)
                {
                    active_manager->destroy_entity(i);
                    continue;
                }

                // Move the projectile:
                transform = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
                transform->position += projectile->direction * float(deltaT) * float(projectile->speed);

                // Test projectile collision:
                unsigned int hitTarget = test_projectile_collision(i, includeSun);
                if (hitTarget == TOO_MANY_ENTITIES) { continue; }
                health = active_manager->get_component<HealthComponent>(hitTarget, COMPONENT_HEALTH);
                weapon = active_manager->get_component<WeaponComponent>(projectile->originID, COMPONENT_WEAPON);
                if (active_manager->has_component(hitTarget, COMPONENT_ARMOUR))
                {
                    armour = active_manager->get_component<ArmourComponent>(hitTarget, COMPONENT_ARMOUR);
                    health->current_health -= weapon->base_damage * ((100.f - armour->armour_value) / 100.f);
                }
                else
                {
                    health->current_health -= weapon->base_damage;
                }
                active_manager->destroy_entity(i);
                continue;
            }
            if (lifeSpanMask->mask[i] == 1)
            {
                // Get the Components:
                lifeSpan = active_manager->get_component<LifeSpanComponent>
                                                    (i, COMPONENT_LIFESPAN);

                // Update the lifetime of the object:
                lifeSpan->currentLifetime += deltaT;

                // If time is up, destroy the Entity:
                if (lifeSpan->currentLifetime >= lifeSpan->totalLifetime)
                {
                    active_manager->destroy_entity(i);
                }
            }
            if (playerMask->mask[i] == 1)
            {
                // Check game mode:
                if (glfwGetInputMode(app_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) { continue; }

                // Get the Components:
                weapon = active_manager->get_component<WeaponComponent>(i, COMPONENT_WEAPON);
                weapon->lastFire += deltaT;

                if (weapon->lastFire >= weapon->attack_speed)
                {
                    // Check for mouse button press:
                    if (GLFW_PRESS == glfwGetMouseButton(app_window, PLAYER_CTRL_SHOOT))
                    {
                        shoot(i, TOO_MANY_ENTITIES, 2);
                    }

                    weapon->lastFire = 0.f;
                }
            }
            if (sunMask->mask[i] == 1)
            {
                // If health is at 0, kill the Sun:
                health = active_manager->get_component<HealthComponent>(i, COMPONENT_HEALTH);
                if (health->current_health <= 0.0)
                {
                    destroy_sun(i);
                    kill_all_enemies();
                }
            }
        }
    }

    void CombatSystem::spawn_enemy(EnemySpawnComponent *enemySpawner, unsigned int spawnerID,
                                                                        float moveSpeedMult)
    {
        // Get the assets path:
        auto path = FileModule::Instance()->get_internal_assets_path();

        // Create a new Entity:
        ECSManager* active_manager = get_manager();
        unsigned int enemyID = active_manager->create_entity();
        if (enemyID == TOO_MANY_ENTITIES) { return; }
        active_manager->set_entityName(enemyID, "Enemy Hubble");

        // Spawn the enemy at the same location as the spawner:
        TransformComponent *spawnerTransform = active_manager->get_component<TransformComponent>(spawnerID, COMPONENT_TRANSFORM);
        TransformComponent *enemyTransform = active_manager->get_component<TransformComponent>(enemyID, COMPONENT_TRANSFORM);
        enemyTransform->position = spawnerTransform->position;
        enemyTransform->scale = glm::vec3(0.15, 0.15, 0.15);

        // Add the enemy Component with move speed and spawner information:
        EnemyComponent enemy { moveSpeedMult, spawnerID, TOO_MANY_ENTITIES };

        // Choose a target planet, if applicable:
        quasarts_entity_ID_mask *planetMask = get_entity_ID_mask(2);
        std::vector<unsigned int> planetIDs;
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (planetMask->mask[i] == 1)
            {
                planetIDs.push_back(i);
            }
        }

        // Now choose a planet at random:
        if (planetIDs.size() > 0)
        {
            int randPlanet = rand() % planetIDs.size();
            enemy.targetID = planetIDs[randPlanet];
        }

        active_manager->create_component(enemyID, COMPONENT_ENEMY, enemy);

        // Create the mesh Component:
        MeshComponent mesh;
        mesh.path = path + "Meshes/Hubble-Telescope.obj";
        ECSManager::Instance()->create_component<MeshComponent>(enemyID, COMPONENT_MESH, mesh);

        // Create the material Component:
        MaterialComponent material;
        std::string texturePath = path + "Texture/metal.png";
        std::string vshPath = path + "Shader/DefaultShader.vsh";
        std::string fshPath = path + "Shader/DefaultShader.fsh";
        std::string gshPth = "";
        material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
        ECSManager::Instance()->create_component<MaterialComponent>(enemyID, COMPONENT_MATERIAL, material);

        // Create the health and weapon Components:
        HealthComponent health { 100.0, 100.0 };
        active_manager->create_component(enemyID, COMPONENT_HEALTH, health);
        WeaponComponent weapon { 1.0, 7.5, 1.0, 0.0 };
        active_manager->create_component(enemyID, COMPONENT_WEAPON, weapon);

        // Increment the spawn count and reset timer:
        enemySpawner->currentSpawns++;
        enemySpawner->lastSpawn = 0.0;

        // Add particle emitter to enemy:
        {
            ParticleComponent particle;
            particle.randomRotation = false;
            particle.is_on = true;
            particle.cone = true;
            particle.pps = 45.f;
            particle.gravity = 0.f;
            particle.averageSpeed = 1.f;
            particle.averageLifeLength = 1.f;
            particle.averageScale = 0.1f;
            particle.speedError = 0.3f;
            particle.lifeError = 0.2f;
            particle.scaleError = 0.01f;
            particle.posError = 0.f;
            particle.direction = glm::vec3(0.f, 0.f, -1.f);
            particle.directionDeviation = 0.3f;
            particle.texture.rows = 4;

            active_manager->create_component(enemyID, COMPONENT_PARTICLE, particle);
        }

        ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(enemyID, COMPONENT_PARTICLE);

        std::string particleTex = path + "Texture/particleAtlas.png";
        particle2->path = particleTex;
        particle2->loadtex(particleTex.c_str());
    }

    void CombatSystem::shoot(unsigned int origin, unsigned int target, int shooter)
    {
        // Get the assets path:
        auto path = FileModule::Instance()->get_internal_assets_path();

        // Create new Entity:
        unsigned int projectileID = ECSManager::Instance()->create_entity();
        if (projectileID == TOO_MANY_ENTITIES) { return; }

        ECSManager::Instance()->set_entityName(projectileID, "Projectile");

        // Get shoot direction:
        TransformComponent *originTransform = ECSManager::Instance()->get_component
                                <TransformComponent>(origin, COMPONENT_TRANSFORM);
        glm::vec3 direction;
        if (shooter == 2) // Player.
        {
            direction.x = (float)(std::cos(glm::radians(originTransform->rotation.y))
                        * std::cos(glm::radians(originTransform->rotation.x)));
            direction.y = (float)(std::sin(glm::radians(originTransform->rotation.x)));
            direction.z = (float)(std::sin(glm::radians(originTransform->rotation.y))
                        * std::cos(glm::radians(originTransform->rotation.x)));
            direction = glm::normalize(direction);
        }
        else
        {
            TransformComponent *targetTransform = ECSManager::Instance()->get_component
                                <TransformComponent>(target, COMPONENT_TRANSFORM);
            direction = targetTransform->position - originTransform->position;
            direction = glm::normalize(direction);
        }

        // Create the projectile Component:
        ProjectileComponent projectile { 10.0, 0.75, 0.0, origin, direction };
        if (shooter == 2) {
            projectile.speed = 75.f;
            projectile.maxTime = 1.f;
        }
        ECSManager::Instance()->create_component(projectileID, COMPONENT_PROJECTILE, projectile);

        // Update the transform to match the shooter:
        TransformComponent *projectileTransform = ECSManager::Instance()->get_component<TransformComponent>
                                                                (projectileID, COMPONENT_TRANSFORM);
        projectileTransform->position = originTransform->position + (0.2f * direction);;
        projectileTransform->scale = glm::vec3(0.04, 0.04, 0.04);

        // Create the mesh Component:
        MeshComponent mesh;
        mesh.path = path + "DefaultObjects/tetrahedron_smooth.obj";
        ECSManager::Instance()->create_component<MeshComponent>(projectileID, COMPONENT_MESH, mesh);

        // Create the material Component:
        MaterialComponent material;
        std::string texturePath;
        if (shooter == 0) { texturePath = path + "Texture/red.png"; }
        else if (shooter == 1) { texturePath = path + "Texture/green.png"; }
        else { texturePath = path + "Texture/blue.png"; }
        std::string vshPath = path + "Shader/DefaultShader.vsh";
        std::string fshPath = path + "Shader/DefaultShader.fsh";
        std::string gshPth = "";
        material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
        if (shooter == 0) { material.material->emission = glm::vec3(1.0, 0.2, 0.2); }
        else if (shooter == 1) { material.material->emission = glm::vec3(0.2, 1.0, 0.2); }
        else { material.material->emission = glm::vec3(0.2, 0.2, 1.0); }
        ECSManager::Instance()->create_component<MaterialComponent>(projectileID, COMPONENT_MATERIAL, material);

        if (shooter == 2)
        {
            glm::vec3 up = glm::vec3(0,1,0);
            glm::vec3 newPosition = glm::normalize(glm::cross(direction, up)) * 0.35f;
            glm::vec3 newDirection = newPosition * 0.1f;

            glm::vec3 newYPosition = glm::normalize(glm::cross(direction, newPosition)) * -0.25f;
            glm::vec3 newYDirection = newYPosition * 0.1f;

            // Spawn another projectile:
            unsigned int projectileID2 = ECSManager::Instance()->create_entity();
            if (projectileID2 == TOO_MANY_ENTITIES) { return; }

            ECSManager::Instance()->set_entityName(projectileID2, "Projectile");

            // Create another projectile Component:
            ProjectileComponent projectile2 { 75.f, 1.f, 0.f, origin, direction + newDirection + newYDirection };
            ECSManager::Instance()->create_component(projectileID2, COMPONENT_PROJECTILE, projectile2);

            // Update the transform to match the shooter:
            TransformComponent *projectileTransform2 = ECSManager::Instance()->get_component<TransformComponent>
                                                                    (projectileID2, COMPONENT_TRANSFORM);
            projectileTransform2->position = projectileTransform->position - newPosition - newYPosition + direction;
            projectileTransform2->scale = glm::vec3(0.04, 0.04, 0.04);

            // Create the mesh Component:
            MeshComponent mesh2;
            mesh2.path = path + "DefaultObjects/tetrahedron_smooth.obj";
            ECSManager::Instance()->create_component<MeshComponent>(projectileID2, COMPONENT_MESH, mesh2);

            // Create the material Component:
            MaterialComponent material2;
            material2.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
            material2.material->emission = glm::vec3(0.2, 0.2, 1.0);
            ECSManager::Instance()->create_component<MaterialComponent>(projectileID2, COMPONENT_MATERIAL, material2);

            // Alter position and direction of original projectile:
            projectileTransform->position += newPosition - newYPosition + direction;
            ProjectileComponent *projectileComp2 = ECSManager::Instance()->get_component<ProjectileComponent>
                                                                        (projectileID, COMPONENT_PROJECTILE);
            projectileComp2->direction -= newDirection - newYDirection;
        }
    }

    void CombatSystem::kill_planet(unsigned int planetID)
    {
        ECSManager *active_manager = get_manager();

        // Remove the health Component so that it doesn't get picked up by the System any more:
        active_manager->destroy_component<HealthComponent>(planetID, COMPONENT_HEALTH);

        // Remove mesh:
        active_manager->destroy_component<MeshComponent>(planetID, COMPONENT_MESH);
        active_manager->destroy_component<MaterialComponent>(planetID, COMPONENT_MATERIAL);

        // Create particle emitter:
        unsigned int emitterID = active_manager->create_entity();
        TransformComponent *transformPlanet = active_manager->get_component<TransformComponent>(planetID, COMPONENT_TRANSFORM);
        TransformComponent *transformEmitter = active_manager->get_component<TransformComponent>(emitterID, COMPONENT_TRANSFORM);
        transformEmitter->position = transformPlanet->position;
        float scale = transformPlanet->scale.x;
        {
            ParticleComponent particle;
            particle.randomRotation = false;
            particle.is_on = true;
            particle.cone = false;
            particle.pps = 90.f;
            particle.gravity = 0.f;
            particle.averageSpeed = scale / 2.f;//1.f;
            particle.averageLifeLength = 2.f;
            particle.averageScale = scale;//0.75f;
            particle.speedError = 0.3f;
            particle.lifeError = 0.2f;
            particle.scaleError = 0.01f;
            particle.posError = 0.f;
            particle.direction = glm::vec3(0.f, 1.f, 0.f);
            particle.directionDeviation = 0.5f;
            particle.texture.rows = 4;

            active_manager->create_component(emitterID, COMPONENT_PARTICLE, particle);
        }

        auto path = FileModule::Instance()->get_internal_assets_path();

        ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(emitterID, COMPONENT_PARTICLE);

        std::string particleTex = path + "Texture/particleAtlas.png";
        particle2->path = particleTex;
        particle2->loadtex(particleTex.c_str());

        LifeSpanComponent life{};
        life.currentLifetime = 0.f;
        life.totalLifetime = 3.f;
        active_manager->create_component(emitterID, COMPONENT_LIFESPAN, life);
    }

    void CombatSystem::destroy_sun(unsigned int sunID)
    {
        AudioSystem::Instance()->playTrack("Sun-Explosion");

        ECSManager *active_manager = get_manager();

        // Remove the health Component so that it doesn't get picked up by the System any more:
        active_manager->destroy_component<HealthComponent>(sunID, COMPONENT_HEALTH);

        // Remove mesh:
        active_manager->destroy_component<MeshComponent>(sunID, COMPONENT_MESH);
        active_manager->destroy_component<MaterialComponent>(sunID, COMPONENT_MATERIAL);

        // Remove Sun particle emitter:
        active_manager->destroy_component<ParticleComponent>(sunID, COMPONENT_PARTICLE);

        // Get Sun position:
        TransformComponent *transformSun = active_manager->get_component<TransformComponent>(sunID, COMPONENT_TRANSFORM);
        TransformComponent *transformEmitter;

        auto path = FileModule::Instance()->get_internal_assets_path();

        // Central emitter:
        {
            unsigned int emitterID = active_manager->create_entity();
            transformEmitter = active_manager->get_component<TransformComponent>(emitterID, COMPONENT_TRANSFORM);
            transformEmitter->position = transformSun->position;
            {
                ParticleComponent particle;
                particle.randomRotation = false;
                particle.is_on = true;
                particle.cone = false;
                particle.pps = 90.f;
                particle.gravity = 0.f;
                particle.averageSpeed = 3.f;
                particle.averageLifeLength = 2.f;
                particle.averageScale = 3.f;
                particle.speedError = 0.3f;
                particle.lifeError = 0.2f;
                particle.scaleError = 0.01f;
                particle.posError = 0.f;
                particle.direction = glm::vec3(0.f, 1.f, 0.f);
                particle.directionDeviation = 0.5f;
                particle.texture.rows = 4;

                active_manager->create_component(emitterID, COMPONENT_PARTICLE, particle);
            }

            auto path = FileModule::Instance()->get_internal_assets_path();

            ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(emitterID, COMPONENT_PARTICLE);

            std::string particleTex = path + "Texture/particleAtlas.png";
            particle2->path = particleTex;
            particle2->loadtex(particleTex.c_str());

            LifeSpanComponent life{};
            life.currentLifetime = 0.f;
            life.totalLifetime = 3.f;
            active_manager->create_component(emitterID, COMPONENT_LIFESPAN, life);
        }

        // Side emitters:
        for (int i = 0; i < 8; i++)
        {
            float degrees = 45.f * (float)(i);
            glm::vec3 direction;
            direction.x = glm::cos(glm::radians(degrees));
            direction.y = 0.f;
            direction.z = glm::sin(glm::radians(degrees));
            direction = glm::normalize(direction);

            unsigned int emitterID = active_manager->create_entity();
            transformEmitter = active_manager->get_component<TransformComponent>(emitterID, COMPONENT_TRANSFORM);
            transformEmitter->position = transformSun->position;
            {
                ParticleComponent particle;
                particle.randomRotation = false;
                particle.is_on = true;
                particle.cone = true;
                particle.pps = 90.f;
                particle.gravity = 0.f;
                particle.averageSpeed = 5.f;
                particle.averageLifeLength = 2.f;
                particle.averageScale = 0.75f;
                particle.speedError = 0.3f;
                particle.lifeError = 0.2f;
                particle.scaleError = 0.01f;
                particle.posError = 0.f;
                particle.direction = direction;
                particle.directionDeviation = 0.5f;
                particle.texture.rows = 4;

                active_manager->create_component(emitterID, COMPONENT_PARTICLE, particle);
            }

            ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(emitterID, COMPONENT_PARTICLE);

            std::string particleTex = path + "Texture/particleAtlas.png";
            particle2->path = particleTex;
            particle2->loadtex(particleTex.c_str());

            LifeSpanComponent life{};
            life.currentLifetime = 0.f;
            life.totalLifetime = 3.f;
            active_manager->create_component(emitterID, COMPONENT_LIFESPAN, life);
        }
    }

    void CombatSystem::kill_all_enemies()
    {
        ECSManager *active_manager = get_manager();

        quasarts_entity_ID_mask *enemies = get_entity_ID_mask(0);
        quasarts_entity_ID_mask *enemySpawners = get_entity_ID_mask(1);

        EnemySpawnComponent *enemySpawn;
        MaterialComponent *material;

        // Loop through all Entities:
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (enemySpawners->mask[i] == 1)
            {
                // Reset and disable the spawner:
                enemySpawn = active_manager->get_component<EnemySpawnComponent>(i, COMPONENT_ENEMY_SPAWNER);
                enemySpawn->isActive = false;
                enemySpawn->currentSpawns = 0;
            }
            else if (enemies->mask[i] == 1)
            {
                // Destroy health Component so that the enemy won't be considered by the mask any more:
                active_manager->destroy_component<HealthComponent>(i, COMPONENT_HEALTH);

                material = active_manager->get_component<MaterialComponent>(i, COMPONENT_MATERIAL);
                material->material->emission = glm::vec3(1.f, 0.f, 0.f);

                LifeSpanComponent life{0.f, 1.f};
                active_manager->create_component<LifeSpanComponent>(i, COMPONENT_LIFESPAN, life);
                {
                    ParticleComponent particle;
                    particle.randomRotation = false;
                    particle.is_on = true;
                    particle.cone = false;
                    particle.pps = 10.f;
                    particle.gravity = 0.f;
                    particle.averageSpeed = 0.4f;
                    particle.averageLifeLength = 2.f;
                    particle.averageScale = 0.2f;
                    particle.speedError = 0.3f;
                    particle.lifeError = 0.2f;
                    particle.scaleError = 0.01f;
                    particle.posError = 0.f;
                    particle.direction = glm::vec3(0.f, 1.f, 0.f);
                    particle.directionDeviation = 0.5f;
                    particle.texture.rows = 4;

                    active_manager->create_component(i, COMPONENT_PARTICLE, particle);
                }

                auto path = FileModule::Instance()->get_internal_assets_path();

                ParticleComponent *particle2 = ECSManager::Instance()->get_component<ParticleComponent>(i, COMPONENT_PARTICLE);

                std::string particleTex = path + "Texture/particleAtlas.png";
                particle2->path = particleTex;
                particle2->loadtex(particleTex.c_str());
            }
        }
    }

    unsigned int CombatSystem::test_projectile_collision(unsigned int projectileID, bool includeSun)
    {
        quasarts_entity_ID_mask *enemies = get_entity_ID_mask(0);
        quasarts_entity_ID_mask *planetMask = get_entity_ID_mask(2);
        quasarts_entity_ID_mask *playerMask = get_entity_ID_mask(5);
        quasarts_entity_ID_mask *sunMask = get_entity_ID_mask(6);

        ProjectileComponent *projectile = ECSManager::Instance()->get_component
                        <ProjectileComponent>(projectileID, COMPONENT_PROJECTILE);
        int originType;
        if (enemies->mask[projectile->originID] == 1) { originType = 0; }           // Enemy origin type.
        else if (planetMask->mask[projectile->originID] == 1) { originType = 1; }   // Friend origin type.
        else if (playerMask->mask[projectile->originID] == 1) { originType = 1; }
        else { return TOO_MANY_ENTITIES; }

        TransformComponent *projectileTransform = ECSManager::Instance()->get_component
                                <TransformComponent>(projectileID, COMPONENT_TRANSFORM);
        TransformComponent *targetTransform;

        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            unsigned short result;
            if (originType == 1) { result = enemies->mask[i]; }
            else if ((originType == 0) && includeSun) { result = planetMask->mask[i] | sunMask->mask[i]; }
            else if ((originType == 0) && !includeSun) { result = planetMask->mask[i]; }

            if (result == 0) { continue; }

            targetTransform =  ECSManager::Instance()->get_component
                        <TransformComponent>(i, COMPONENT_TRANSFORM);

            glm::vec3 distVec = targetTransform->position - projectileTransform->position;
            float dist = glm::length(distVec);

            if (dist <= (targetTransform->scale.x + 0.1f))
            {
                return i;
            }
        }

        return TOO_MANY_ENTITIES;
    }
}