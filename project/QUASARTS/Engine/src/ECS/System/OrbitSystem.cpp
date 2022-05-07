#include "OrbitSystem.h"


namespace Engine
{

	OrbitSystem::OrbitSystem() :
		mNumOrbits(0)
	{

	} // OrbitSystem()

	OrbitSystem::~OrbitSystem()
	{

	} // ~OrbitSystem()
	

	void OrbitSystem::init()
	{

	} // init()


	int OrbitSystem::start()
	{
		ECSManager* active_manager = get_manager();

		// Initialise all orbits with valid primaries.
		quasarts_entity_ID_mask *entitiesOrbits = get_entity_ID_mask(0);
		OrbitComponent *orbit;
		TransformComponent *transf, *transfPrimary;
		glm::vec3 relativePos;
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (entitiesOrbits->mask[i] == 1) // Entity [i] with orbit component.
			{
				orbit = active_manager->get_component
					<OrbitComponent>(i, COMPONENT_ORBIT);

				// If primary is valid.
				if (orbit->mPrimaryEntityId != -1)
				{
					transf = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
					transfPrimary = active_manager->get_component<TransformComponent>(orbit->mPrimaryEntityId, COMPONENT_TRANSFORM);

					// Initialise with relative position.
					relativePos = transf->position - transfPrimary->position;
					orbit->init_orbit(relativePos);
				}
			}
		}

		return 0;

	} // start()

	void OrbitSystem::update()
	{
		ECSManager* active_manager = get_manager();

		// Update all collision object positions for next physics update.
		quasarts_entity_ID_mask *entitiesOrbits = get_entity_ID_mask(0);
		OrbitComponent *orbit;
		TransformComponent *transf, *transfPrimary;
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (entitiesOrbits->mask[i] == 1)
			{
				// The entity is valid for the System:
				orbit = active_manager->get_component<OrbitComponent>(i, COMPONENT_ORBIT);

				transf = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
				transfPrimary = active_manager->get_component<TransformComponent>(orbit->mPrimaryEntityId, COMPONENT_TRANSFORM);

				// TODO : update positions
			}
		}

	} // update()


	int OrbitSystem::stop()
	{
		return 0;

	} // stop()


	void OrbitSystem::release()
	{

	} // release()


	// Usage //

	void OrbitSystem::set_orbit_primary(unsigned int const aEntityId, unsigned int const aPrimaryEntityId)
	{
		ECSManager* active_manager = get_manager();

		// Get the entity's orbit component.
		OrbitComponent* orbit = active_manager->get_component<OrbitComponent>(aEntityId, COMPONENT_ORBIT);
		if (nullptr == orbit)
		{
			QDEBUG("Cannot set orbit primary: could not find orbit component in entity with ID: {0}", aEntityId);
			return;
		}

		// Get world position of this entity relative to the primary.
		TransformComponent* transf = active_manager->get_component<TransformComponent>(aEntityId, COMPONENT_TRANSFORM);
		TransformComponent* primTransf = active_manager->get_component<TransformComponent>(aPrimaryEntityId, COMPONENT_TRANSFORM);

		// Check primary exists.
		if (nullptr == primTransf)
		{
			QDEBUG("Cannot set orbit primary: could not get entity with ID: {0}", aPrimaryEntityId);
			return;
		}

		// Add this entity to the hierarchy



		// Set orbit primary.
		orbit->set_primary(aPrimaryEntityId);

	}

}