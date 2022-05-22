#include "EnemySystem.h"

namespace Engine
{
    // Singleton:
    EnemySystem* EnemySystem::instance = nullptr;

    // Create the instance of the EnemySystem:
    EnemySystem* EnemySystem::Instance() {
        if (nullptr == instance)
            instance = new EnemySystem();
        return instance;
    }

    EnemySystem::EnemySystem() {
        // Set the Component mask for enemies:
        quasarts_component_mask mask1;
        mask1.mask = 0;
        mask1.mask += (uint64_t)1 << COMPONENT_ENEMY;
        mask1.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
        mask1.mask += (uint64_t)1 << COMPONENT_MESH;
        mask1.mask += (uint64_t)1 << COMPONENT_MATERIAL;
        mask1.mask += (uint64_t)1 << COMPONENT_HEALTH;
        mask1.mask += (uint64_t)1 << COMPONENT_WEAPON;
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

        // Seed random generator with current time:
        srand(time(nullptr));
    }

    void EnemySystem::update()
    {
        // Get manager, declare Component pointers and get masks:
        ECSManager* active_manager = get_manager();
        EnemyComponent *enemy;
        EnemySpawnComponent *enemySpawner;
        TransformComponent *transform;
        HealthComponent *health;
        WeaponComponent *weapon;
        ProjectileComponent *projectile;
        quasarts_entity_ID_mask *enemies = get_entity_ID_mask(0);
        quasarts_entity_ID_mask *enemySpawners = get_entity_ID_mask(1);
        quasarts_entity_ID_mask *planetMask = get_entity_ID_mask(2);
        quasarts_entity_ID_mask *projectileMask = get_entity_ID_mask(3);

        // Check the time:
        auto deltaT = TimeModule::Instance()->get_frame_delta_time().sec();

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

                // Check health:
                if (health->current_health <= 0.0)
                {
                    QDEBUG("Enemy {0} died.", i);
                    unsigned int numEntities = active_manager->get_num_entities();
                    active_manager->destroy_entity(i);
                    //AudioSystem::Instance()->playSoundClip("Test/Explosion");
                    enemySpawner->currentSpawns--;
                    // Remove all projectiles that reference this Entity:
                    for (int j = 0; j < MAX_ENTITIES; j++)
                    {
                        if (projectileMask->mask[j] == 1)
                        {
                            projectile = active_manager->get_component<ProjectileComponent>(j, COMPONENT_PROJECTILE);
                            if (projectile->targetID == i)
                            {
                                active_manager->destroy_entity(j);
                                continue;
                            }
                            if (projectile->originID == i)
                            {
                                active_manager->destroy_entity(j);
                            }
                        }
                    }
                    continue;
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

                // If still no target, skip:
                if (enemy->targetID == TOO_MANY_ENTITIES) { continue; }

                // Find the target position:
                TransformComponent *targetTransform = active_manager->get_component
                        <TransformComponent>(enemy->targetID, COMPONENT_TRANSFORM);

                // Move the enemy towards the target:
                glm::vec3 targetDirection = targetTransform->position - transform->position;
                float targetDistance = glm::length(targetDirection);
                float directionMult;
                if (targetDistance <= 3.5) { directionMult = -1.0; }
                else if (targetDistance <= 4.0) { directionMult = 0.0; }
                else { directionMult = 1.0; }

                transform->position += glm::normalize(targetDirection) * float(deltaT)
                                * float(directionMult) * float(enemy->moveSpeedMult);

                // Check the last time the enemy fired a projectile:
                weapon->lastFire += deltaT;
                if (weapon->lastFire < weapon->attack_speed) { continue; }
                if (targetDistance <= 5.0)
                {
                    // Fire a projectile at the target:
                    shoot(i, enemy->targetID, true);
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
                    //continue;
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

                            if (distance <= 5.0)
                            {
                                // Fire a projectile at the enemy:
                                shoot(i, j, false);
                                weapon->lastFire = 0.0;
                                break;
                            }
                        }
                    }
                }
            }

            if (projectileMask->mask[i] == 1)
            {
                // Check proximity to target:
                transform = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
                projectile = active_manager->get_component<ProjectileComponent>(i, COMPONENT_PROJECTILE);
                TransformComponent *targetTransform = active_manager->get_component<TransformComponent>
                                                            (projectile->targetID, COMPONENT_TRANSFORM);
                health = active_manager->get_component<HealthComponent>(projectile->targetID, COMPONENT_HEALTH);
                weapon = active_manager->get_component<WeaponComponent>(projectile->originID, COMPONENT_WEAPON);

                projectile->currentTime += deltaT;

                // Seeking projectile, not straight path:
                glm::vec3 projectilePath = targetTransform->position - transform->position;

                float distance = glm::length(projectilePath);

                if (distance < 0.1)
                {
                    // Hit:
                    health->current_health -= weapon->base_damage;
                    active_manager->destroy_entity(i);
                    continue;
                }

                // If the time is up, expire:
                if (projectile->currentTime >= projectile->maxTime)
                {
                    active_manager->destroy_entity(i);
                    continue;
                }

                // Move the projectile:
                transform->position += glm::normalize(projectilePath) * float(deltaT)
                                                            * float(projectile->speed);
            }
        }
    }

    void EnemySystem::spawn_enemy(EnemySpawnComponent *enemySpawner, unsigned int spawnerID,
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
        WeaponComponent weapon { 10.0, 1.5, 1.0, 0.0 };
        active_manager->create_component(enemyID, COMPONENT_WEAPON, weapon);

        // Increment the spawn count and reset timer:
        enemySpawner->currentSpawns++;
        enemySpawner->lastSpawn = 0.0;
    }

    void EnemySystem::shoot(unsigned int origin, unsigned int target, bool shootAtPlanet)
    {
        // Get the assets path:
        auto path = FileModule::Instance()->get_internal_assets_path();

        // Create new Entity:
        unsigned int projectileID = ECSManager::Instance()->create_entity();
        if (projectileID == TOO_MANY_ENTITIES) { return; }

        ECSManager::Instance()->set_entityName(projectileID, "Projectile");

        // Create the projectile Component:
        ProjectileComponent projectile { 10.0, 0.75, 0.0, origin, target };
        ECSManager::Instance()->create_component(projectileID, COMPONENT_PROJECTILE, projectile);

        // Update the transform to match the shooter:
        TransformComponent *originTransform = ECSManager::Instance()->get_component<TransformComponent>
                                                                (origin, COMPONENT_TRANSFORM);
        TransformComponent *projectileTransform = ECSManager::Instance()->get_component<TransformComponent>
                                                                (projectileID, COMPONENT_TRANSFORM);
        projectileTransform->position = originTransform->position;
        projectileTransform->scale = glm::vec3(0.04, 0.04, 0.04);

        // Create the mesh Component:
        MeshComponent mesh;
        mesh.path = path + "DefaultObjects/tetrahedron_smooth.obj";
        ECSManager::Instance()->create_component<MeshComponent>(projectileID, COMPONENT_MESH, mesh);

        // Create the material Component:
        MaterialComponent material;
        std::string texturePath;
        if (shootAtPlanet == true) { texturePath = path + "Texture/red.png"; }
        else { texturePath = path + "Texture/green.png"; }
        std::string vshPath = path + "Shader/DefaultShader.vsh";
        std::string fshPath = path + "Shader/DefaultShader.fsh";
        std::string gshPth = "";
        material.material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
        if (shootAtPlanet == true) { material.material->emission = glm::vec3(1.0, 0.2, 0.2); }
        else { material.material->emission = glm::vec3(0.2, 1.0, 0.2); }
        ECSManager::Instance()->create_component<MaterialComponent>(projectileID, COMPONENT_MATERIAL, material);

        // Play a laser noise:
        AudioSystem::Instance()->playSoundClip("laser1");
    }

    void EnemySystem::kill_planet(unsigned int planetID)
    {
        HealthComponent *health = ECSManager::Instance()->get_component<HealthComponent>(planetID, COMPONENT_HEALTH);
        health->current_health = 0.0;

        // Play an explosion noise:
        //AudioSystem::Instance()->playSoundClip("Test/Explosion");
    }
}