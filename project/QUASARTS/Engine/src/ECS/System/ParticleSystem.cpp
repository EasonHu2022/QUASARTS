#include "ParticleSystem.h"


namespace Engine
{
	ParticleSystem::ParticleSystem()
	{

		quasarts_component_mask mask;
		mask.mask = 0;
		mask.mask += (uint64_t)1 << COMPONENT_PARTICLE;
		mask.mask += (uint64_t)1 << COMPONENT_TRANSFORM;
		add_component_mask(mask);

	}

	ParticleSystem::~ParticleSystem()
	{

	}

	void ParticleSystem::update() {

		ECSManager* mgr = get_manager();

		quasarts_entity_ID_mask* ent = get_entity_ID_mask(0);

		ParticleComponent* particle;
		TransformComponent* transform;

		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (ent->mask[i] == 1)
			{
				particle = mgr->get_component<ParticleComponent>(i, COMPONENT_PARTICLE);
				transform = mgr->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
				particle->generateParticle(transform->position);
			}
		}
	}

};

