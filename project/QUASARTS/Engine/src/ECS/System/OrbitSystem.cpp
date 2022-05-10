#include "OrbitSystem.h"

#include <sstream>


namespace Engine
{

	OrbitSystem::OrbitSystem()
	{
		mOrbitRoot = std::make_shared<OrbitNode>();

	} // OrbitSystem()

	OrbitSystem::~OrbitSystem()
	{
		// Delete orbit tree.
		for (auto node : mAllOrbitNodes)
		{
			node.second->pPrimaryNode = nullptr;
			node.second->mSatelliteNodes.clear();
		}
		mAllOrbitNodes.clear();

	} // ~OrbitSystem()
	

	void OrbitSystem::init()
	{
		mOrbitRoot->mEntityId = -1;
		mOrbitRoot->mPrimaryId = -1;
		mOrbitRoot->pPrimaryNode = nullptr;
		for (auto node : mAllOrbitNodes)
		{
			node.second->pPrimaryNode = nullptr;
			node.second->mSatelliteNodes.clear();
		}
		mAllOrbitNodes.clear();


		//tree_tests();

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

				// Start orbital motion.
				if (orbit->mPrimaryEntityId != -1)
				{
					transf = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
					transfPrimary = active_manager->get_component<TransformComponent>(orbit->mPrimaryEntityId, COMPONENT_TRANSFORM);

					// Initialise orbit shape with current relative position.
					relativePos = transf->position - transfPrimary->position;
					orbit->start_orbit(relativePos);
				}
			}
		}

		return 0;

	} // start()

	void OrbitSystem::update()
	{
		ECSManager* active_manager = get_manager();

		// Iterate through the orbit tree to update the positions of all entities with orbital motion.
		quasarts_entity_ID_mask *entitiesOrbits = get_entity_ID_mask(0);
		std::shared_ptr<OrbitNode> node;
		OrbitComponent *orbit;
		TransformComponent *transf, *transfPrimary;

		// TODO : iterate through tree


	} // update()


	int OrbitSystem::stop()
	{
		return 0;

	} // stop()


	void OrbitSystem::release()
	{

	} // release()


	// Usage //

	int OrbitSystem::set_orbit_primary(unsigned int const aEntityId, unsigned int const aPrimaryEntityId)
	{
		ECSManager* active_manager = get_manager();

		// Get the entity's orbit component.
		OrbitComponent* orbit = active_manager->get_component<OrbitComponent>(aEntityId, COMPONENT_ORBIT);
		if (nullptr == orbit)
		{
			QERROR("Cannot set orbit primary for entity {0}: could not find orbit component.", aEntityId);
			return -1;
		}

		// Check primary exists.
		if (nullptr == active_manager->get_entity(aPrimaryEntityId))
		{
			// If primary is invalid, clear existing orbit (removes node from tree).
			clear_orbit(aEntityId);
			QERROR("Cannot set orbit primary for entity {0}: could not find primary entity: {1}. Orbit cleared.", aEntityId, aPrimaryEntityId);
			return -1;
		}


		// Adding this entity to the orbit tree //
		std::shared_ptr<OrbitNode> orbitNode;

		// Get this entity's orbit node.
		if (auto nodeIt = mAllOrbitNodes.find(aEntityId); nodeIt == mAllOrbitNodes.end())
		{
			// If node does not exist, create it.
			orbitNode = std::make_shared<OrbitNode>();
			orbitNode->mEntityId = aEntityId;

			// Map entity to its new node.
			mAllOrbitNodes.insert({ aEntityId, orbitNode });
		}
		else
		{
			// Otherwise, get node.
			orbitNode = nodeIt->second;

			if (nullptr == orbitNode->pPrimaryNode)
			{
				// If node is inactive, update any satellite nodes - any nodes
				// whose orbits had this entity as their primary would have set
				// the root as their parent.
				for (auto node : mAllOrbitNodes)
				{
					if (node.second->mPrimaryId == aEntityId)
					{
						node.second->pPrimaryNode = orbitNode;
						mOrbitRoot->mSatelliteNodes.erase(node.second);
						orbitNode->mSatelliteNodes.insert(node.second);
					}
				}
			}				
			else
			{
				// Otherwise, remove it from its parent's children.
				orbitNode->pPrimaryNode->mSatelliteNodes.erase(orbitNode);
			}
		}

		// Get and set the node's new parent.
		orbitNode->mPrimaryId = aPrimaryEntityId;
		if (auto nodeIt = mAllOrbitNodes.find(aPrimaryEntityId); nodeIt == mAllOrbitNodes.end())
		{
			// If the primary does not have a node, set the parent to the root node.
			orbitNode->pPrimaryNode = mOrbitRoot;
		}
		else
		{
			// Otherwise, set the parent to the primary's node.
			orbitNode->pPrimaryNode = nodeIt->second;
		}
		// Add the node to the new parent's children.
		orbitNode->pPrimaryNode->mSatelliteNodes.insert(orbitNode);

		// In the entity's orbit component, set the new primary.
		orbit->mPrimaryEntityId = aPrimaryEntityId;

		return 0;

	} // set_orbit_primary()


	void OrbitSystem::clear_orbit(unsigned int const aEntityId)
	{
		ECSManager* active_manager = get_manager();

		// Remove this entity's node from the orbit tree, but keep node
		// while the orbit component still exists.
		std::shared_ptr<OrbitNode> orbitNode;
		if (auto nodeIt = mAllOrbitNodes.find(aEntityId); nodeIt != mAllOrbitNodes.end())
		{
			orbitNode = nodeIt->second;

			orbitNode->mPrimaryId = -1;
			orbitNode->pPrimaryNode->mSatelliteNodes.erase(orbitNode);
			orbitNode->pPrimaryNode = nullptr;

			// Set children's parent to the root.
			// Children do not inherit the node's parent because they
			// still orbit this entity.
			for (auto child : orbitNode->mSatelliteNodes)
			{
				child->pPrimaryNode = mOrbitRoot;
				mOrbitRoot->mSatelliteNodes.insert(child);
			}
			orbitNode->mSatelliteNodes.clear();
		}

		// Clear orbit component.
		OrbitComponent* orbit = active_manager->get_component<OrbitComponent>(aEntityId, COMPONENT_ORBIT);
		if (nullptr == orbit)
		{
			QERROR("Cannot clear orbit component belonging to entity {0}: could not find orbit component.", aEntityId);
			return;
		}
		orbit->clear();

	} // clear_orbit()


	void OrbitSystem::destroy_orbit(unsigned int const aEntityId)
	{
		// Safely remove the node from the tree.
		clear_orbit(aEntityId);

		ECSManager* active_manager = get_manager();

		// Destroy the node.
		if (auto nodeIt = mAllOrbitNodes.find(aEntityId); nodeIt != mAllOrbitNodes.end())
		{
			// Erase mapping.
			mAllOrbitNodes.erase(nodeIt);
		}

	} // destroy_orbit()



	// Util //
	
	std::shared_ptr<OrbitSystem::OrbitNode> OrbitSystem::get_node(unsigned int const aEntityId)
	{
		for (auto node : mAllOrbitNodes)
		{
			if (node.second->mEntityId == aEntityId)
				return node.second;
		}
		return nullptr;
	}


	// Debug //

	std::string OrbitSystem::print_tree()
	{
		std::ostringstream ostr;
		ostr << "orbit tree:\nTODO : print nodes";
		return ostr.str();
	} // print_tree()


	std::string OrbitSystem::OrbitNode::tostring()
	{
		std::ostringstream ostr;
		ostr << "entity ID: " << mEntityId << ", primary ID: " << mPrimaryId << ", satellites: " << mSatelliteNodes.size() << " ( ";
		for (auto sat : mSatelliteNodes)
		{
			ostr << sat->mEntityId << " ";
		}
		ostr << "), parent ID: ";
		if (pPrimaryNode != nullptr) ostr << pPrimaryNode->mEntityId;

		return ostr.str();
	}


	void OrbitSystem::tree_tests()
	{
		ECSManager* active_manager = get_manager();

		// Create entity with orbit component.
		unsigned int entityId = active_manager->create_entity();
		Entity* entity = active_manager->get_entity(entityId);
		if (entity == nullptr) {
			QDEBUG("OrbitSystem: tree_tests(): Failed to create/retrieve entity {0}", entityId); return;
		}
		QDEBUG("OrbitSystem: tree_tests(): Created and retrieved entity {0}", entityId);

		active_manager->create_component<OrbitComponent>(entityId, COMPONENT_ORBIT);
		OrbitComponent* orbit = active_manager->get_component<OrbitComponent>(entityId, COMPONENT_ORBIT);
		if (orbit == nullptr) {
			QDEBUG("OrbitSystem: tree_tests(): Failed to create/retrieve orbit", entityId); return;
		}
		QDEBUG("OrbitSystem: tree_tests(): Created and retrieved orbit", entityId);


		// Create primary.
		unsigned int primEntityId = active_manager->create_entity();
		Entity* primEntity = active_manager->get_entity(primEntityId);
		if (entity == nullptr) {
			QDEBUG("OrbitSystem: tree_tests(): Failed to create/retrieve primary entity {0}", primEntityId); return;
		}
		QDEBUG("OrbitSystem: tree_tests(): Created and retrieved primary entity {0}", primEntityId);


		// Add entity to orbit tree.
		if (0 != set_orbit_primary(entityId, primEntityId)) {
			QDEBUG("OrbitSystem: tree_tests(): Failed to set orbit primary"); return;
		}
		QDEBUG("OrbitSystem: tree_tests(): Successfully set orbit primary. New node: {0}", get_node(entityId)->tostring());


		// Create satellite(s).
		QDEBUG("Creating satellites...");
		std::vector<unsigned int> satEntityIds;
		for (int i = 0; i < 3; ++i)
		{
			unsigned int satEntityId = active_manager->create_entity();
			Entity* satEntity = active_manager->get_entity(satEntityId);
			if (satEntity == nullptr) {
				QDEBUG("OrbitSystem: tree_tests(): Failed to create/retrieve satellite entity {0}", satEntityId); return;
			}
			QDEBUG("OrbitSystem: tree_tests(): Created and retrieved satellite entity {0}", satEntityId);

			active_manager->create_component<OrbitComponent>(satEntityId, COMPONENT_ORBIT);
			OrbitComponent* satOrbit = active_manager->get_component<OrbitComponent>(satEntityId, COMPONENT_ORBIT);
			if (satOrbit == nullptr) {
				QDEBUG("OrbitSystem: tree_tests(): Failed to create/retrieve satellite orbit", satEntityId); return;
			}
			QDEBUG("OrbitSystem: tree_tests(): Created and retrieved satellite orbit", satEntityId);

			if (0 != set_orbit_primary(satEntityId, entityId)) {
				QDEBUG("OrbitSystem: tree_tests(): Failed to set satellite orbit primary"); return;
			}
			QDEBUG("OrbitSystem: tree_tests(): Successfully set satellite orbit primary. New node: {0}", get_node(satEntityId)->tostring());

			satEntityIds.push_back(satEntityId);
		}
		QDEBUG("Updated original node: {0}", get_node(entityId)->tostring());


		// Clear original orbit.
		QDEBUG("Root: {0}", mOrbitRoot->tostring());
		QDEBUG("Clearing orbit of entity {0}...", entityId);
		clear_orbit(entityId);
		QDEBUG("New root: {0}", mOrbitRoot->tostring());
		QDEBUG("Updated node: {0}", get_node(entityId)->tostring());
		QDEBUG("Updated satellites:");
		for (int i = 0; i < satEntityIds.size(); ++i)
		{
			QDEBUG("- sat. #{0}: {1}", i, get_node(satEntityIds[i])->tostring());
		}


		// Clear a satellite orbit.
		QDEBUG("Setting satellite {0} orbit to original primary {1}", satEntityIds[0], primEntityId);
		if (0 != set_orbit_primary(satEntityIds[0], primEntityId)) {
			QDEBUG("OrbitSystem: tree_tests(): Failed to set satellite orbit primary"); return;
		}
		QDEBUG("Updated sat. node: {0}", get_node(satEntityIds[0])->tostring());


		// Re-setting original node.
		QDEBUG("Setting original entity {0} orbit to original primary {1}", entityId, primEntityId);
		if (0 != set_orbit_primary(entityId, primEntityId)) {
			QDEBUG("OrbitSystem: tree_tests(): Failed to set satellite orbit primary"); return;
		}
		QDEBUG("New root: {0}", mOrbitRoot->tostring());
		QDEBUG("Updated node: {0}", get_node(entityId)->tostring());
		QDEBUG("Updated satellites:");
		for (int i = 0; i < satEntityIds.size(); ++i)
		{
			QDEBUG("- sat. #{0}: {1}", i, get_node(satEntityIds[i])->tostring());
		}


		// Deleting original node.
		QDEBUG("Deleting original entity {0} orbit...");
		destroy_orbit(entityId);
		QDEBUG("New root: {0}", mOrbitRoot->tostring());
		QDEBUG("Nodes:");
		for (auto it : mAllOrbitNodes)
		{
			QDEBUG("- {0}", it.second->tostring());
		}


	} // tree_tests()

}