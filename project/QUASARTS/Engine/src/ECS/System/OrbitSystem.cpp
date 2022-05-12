#include "OrbitSystem.h"

#include <sstream>


namespace Engine
{

	OrbitSystem::OrbitSystem()
	{
		// Set the Component mask:
		quasarts_component_mask mask;
		mask.mask = 0;
		mask.mask += (uint64_t)1 << COMPONENT_ORBIT;
		add_component_mask(mask);

		// Create orbit tree root.
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
		//component_tests();

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
				orbit = active_manager->get_component<OrbitComponent>(i, COMPONENT_ORBIT);

				// Start orbital motion.
				if (orbit->mPrimaryEntityId != -1)
				{
					transf = active_manager->get_component<TransformComponent>(i, COMPONENT_TRANSFORM);
					transfPrimary = active_manager->get_component<TransformComponent>(orbit->mPrimaryEntityId, COMPONENT_TRANSFORM);

					// Initialise orbit parameters with current relative position.
					relativePos = transf->position - transfPrimary->position;

					orbit->mRelativePos = relativePos;
					orbit->mDistPeriapse = glm::length(relativePos);
					orbit->mDistance = orbit->mDistPeriapse;
					orbit->mTrueAnom = 0;

					// Define the basis around the relative position vector.
					// Keeps to the existing normal as closely as possible.
					orbit->mAxisX = glm::normalize(relativePos);
					if (orbit->mAxisY = glm::cross(orbit->mAxisNormal, orbit->mRelativePos);
						glm::length(orbit->mAxisY) == 0)
					{
						// Existing normal lies on the relative position axis.
						orbit->mAxisY = OrbitComponent::altY;
					}
					else
					{
						orbit->mAxisY = glm::normalize(orbit->mAxisY);
					}
					orbit->mAxisNormal = glm::normalize(glm::cross(orbit->mAxisX, orbit->mAxisY));
				}

				QDEBUG("Entity {0} orbit: {1}", i, orbit->to_string());
			}
		}

		/*completeTimesCounter = 30;
		for (auto& tracker : orbitTrackers)
		{
			tracker.second.aveDistErr.clear();
			tracker.second.completeTimes.clear();
			tracker.second.maxDistErr = 0;
			tracker.second.minDistErr = 0;
			tracker.second.tick = false;
		}*/

		return 0;

	} // start()

	void OrbitSystem::update()
	{
		if (Input::get_key_pressed(Q_KEY_P))
		{
			if (paused) start(); // restart system when unpaused
			paused = !paused;
		}
		if (paused) return;
		QDEBUG("Running");

		ECSManager* active_manager = get_manager();

		// Iterate through the orbit tree to update the positions of all entities with orbital motion.
		quasarts_entity_ID_mask *entitiesOrbits = get_entity_ID_mask(0);
		std::shared_ptr<OrbitNode> node;
		OrbitComponent *orbit;
		TransformComponent *transf, *transfPrimary;

		constexpr double Pi2 = glm::two_pi<double>();

		double dtPi2 = TimeModule::Instance()->get_frame_delta_time().sec() * Pi2;
		float deltaTheta, trueAnom;

		std::queue<std::shared_ptr<OrbitNode>> nodeQueue;
		for (auto sat : mOrbitRoot->mSatelliteNodes)
		{
			nodeQueue.push(sat);
		}
		while (!nodeQueue.empty())
		{
			// Update queue.
			node = nodeQueue.front();
			nodeQueue.pop();
			for (auto sat : node->mSatelliteNodes)
			{
				nodeQueue.push(sat);
			}

			// debug
			OrbitTracker* tracker = &orbitTrackers[node->mEntityId];

			// Update node entity position. //
			orbit = active_manager->get_component<OrbitComponent>(node->mEntityId, COMPONENT_ORBIT);

			// Get new true anomaly.
			deltaTheta = (orbit->mOrbitPeriod > 0) ? dtPi2 / orbit->mOrbitPeriod : 0;
			trueAnom = orbit->mTrueAnom + deltaTheta;
			if (trueAnom >= Pi2) {
				trueAnom -= Pi2;

				// debug
				//tracker->tick = true;
			}
			orbit->mTrueAnom = trueAnom;

			// Get new relative position.
			orbit->mRelativePos = orbit->mDistPeriapse * ((glm::cos(trueAnom) * orbit->mAxisX) + (glm::sin(trueAnom) * orbit->mAxisY));

			// Set new world position.
			transf = active_manager->get_component<TransformComponent>(node->mEntityId, COMPONENT_TRANSFORM);
			transfPrimary = active_manager->get_component<TransformComponent>(node->mPrimaryId, COMPONENT_TRANSFORM);
			transf->position = transfPrimary->position + orbit->mRelativePos;


			//// stats tracking
			//float actDistErr = glm::length(transfPrimary->position - transf->position) - orbit->mDistPeriapse;
			//if (actDistErr > tracker->maxDistErr) tracker->maxDistErr = actDistErr;
			//if (actDistErr < tracker->minDistErr) tracker->minDistErr = actDistErr;
			//if (tracker->tick)
			//{
			//	tracker->completeTimes.push_back(TimeModule::Instance()->get_time());
			//	tracker->aveDistErr.push_back(actDistErr);
			//	tracker->tick = false;
			//}
		}


		//// debug
		//if (completeTimesCounter.sec() < 0)
		//{
		//	QDEBUG("Orbit stats:");
		//	// Iterate over entities.
		//	for (auto& tracker : orbitTrackers)
		//	{
		//		// Timing
		//		QTime sum = 0;
		//		for (size_t n = 1; n < tracker.second.completeTimes.size(); ++n)
		//		{
		//			sum += tracker.second.completeTimes[n] - tracker.second.completeTimes[n - 1];
		//		}
		//		// Distance error
		//		float sumDistErr = 0;
		//		for (float err : tracker.second.aveDistErr)
		//		{
		//			sumDistErr += err;
		//		}

		//		QDEBUG("- entity {0}: timing: {1}, distance av.err: {2}, max err: {3}, min err: {4}",
		//			tracker.first,
		//			(sum.sec() / (tracker.second.completeTimes.size() - 1)),
		//			sumDistErr / tracker.second.aveDistErr.size(),
		//			tracker.second.maxDistErr,
		//			tracker.second.minDistErr
		//		);
		//	}
		//	completeTimesCounter += 30;
		//}
		//completeTimesCounter -= TimeModule::Instance()->get_frame_delta_time();

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


	void OrbitSystem::set_orbit_period(unsigned int const aEntityId, double const aOrbitPeriod)
	{
		ECSManager* active_manager = get_manager();

		// Get the entity's orbit component.
		OrbitComponent* orbit = active_manager->get_component<OrbitComponent>(aEntityId, COMPONENT_ORBIT);
		if (nullptr == orbit)
		{
			QERROR("Cannot set orbit period for entity {0}: could not find orbit component.", aEntityId);
			return;
		}
		orbit->mOrbitPeriod = abs(aOrbitPeriod);

	} // set_orbit_period()


	void OrbitSystem::set_orbit_normal(unsigned int const aEntityId, glm::vec3 const aNormal)
	{
		ECSManager* active_manager = get_manager();

		// Get the entity's orbit component.
		OrbitComponent* orbit = active_manager->get_component<OrbitComponent>(aEntityId, COMPONENT_ORBIT);
		if (nullptr == orbit)
		{
			QERROR("Cannot set orbit normal for entity {0}: could not find orbit component.", aEntityId);
			return;
		}
		orbit->mAxisNormal = glm::normalize(aNormal);

	} // set_orbit_normal()


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






	// Debug // ***************************************************************************

	void OrbitSystem::component_tests()
	{
		QDEBUG("OrbitSystem::component_tests()");
		ECSManager* active_manager = get_manager();

		// Create entity 0 with orbit component.
		QDEBUG("Creating entity with orbit component...");
		unsigned int entity0Id = active_manager->create_entity();
		Entity* entity0 = active_manager->get_entity(entity0Id);
		if (entity0 == nullptr) {
			QDEBUG("Failed to create/retrieve entity: {0}", entity0Id);
			return;
		}

		active_manager->create_component<OrbitComponent>(entity0Id, COMPONENT_ORBIT);
		OrbitComponent* orbit0 = active_manager->get_component<OrbitComponent>(entity0Id, COMPONENT_ORBIT);
		if (orbit0 == nullptr) {
			QDEBUG("Failed to create/retrieve orbit component for entity: {0}", entity0Id);
			return;
		}

		QDEBUG("Created entity: {0}, with orbit component.", entity0Id);


		// Create and set orbit primary.
		QDEBUG("Creating blank entity...");
		unsigned int entity1Id = active_manager->create_entity();
		Entity* entity1 = active_manager->get_entity(entity1Id);
		if (entity1 == nullptr) {
			QDEBUG("Failed to create/retrieve entity: {0}", entity1Id);
			return;
		}
		QDEBUG("Created entity: {0}.", entity1Id);

		QDEBUG("Setting entity {0} orbit primary to entity {1}...", entity0Id, entity1Id);
		set_orbit_primary(entity0Id, entity1Id);
		QDEBUG("Entity {0} new orbit primary: {1}", entity0Id, orbit0->mPrimaryEntityId);


		// Create entity 2 with orbit component and primary 0.
		QDEBUG("Creating entity with orbit component...");
		unsigned int entity2Id = active_manager->create_entity();
		Entity* entity2 = active_manager->get_entity(entity2Id);
		if (entity2 == nullptr) {
			QDEBUG("Failed to create/retrieve entity: {0}", entity2Id);
			return;
		}
		active_manager->create_component<OrbitComponent>(entity2Id, COMPONENT_ORBIT);
		OrbitComponent* orbit2 = active_manager->get_component<OrbitComponent>(entity2Id, COMPONENT_ORBIT);
		if (orbit2 == nullptr) {
			QDEBUG("Failed to create/retrieve orbit component for entity: {0}", entity2Id);
			return;
		}
		QDEBUG("Created entity: {0}, with orbit component.", entity2Id);
		QDEBUG("Setting entity {0} orbit primary to entity {1}...", entity2Id, entity0Id);
		set_orbit_primary(entity2Id, entity0Id);
		QDEBUG("Entity {0} new orbit primary: {1}", entity2Id, orbit2->mPrimaryEntityId);


		// Offset entities from primaries.
		TransformComponent* transform0 = active_manager->get_component<TransformComponent>(entity0Id, COMPONENT_TRANSFORM);
		TransformComponent* transform1 = active_manager->get_component<TransformComponent>(entity1Id, COMPONENT_TRANSFORM);
		TransformComponent* transform2 = active_manager->get_component<TransformComponent>(entity2Id, COMPONENT_TRANSFORM);
		glm::vec3 startRelativePos0(10, 0, 10);
		glm::vec3 startRelativePos2(0, -5, 0);
		QDEBUG("Setting entity {0} relative position...", entity0Id);
		transform0->position = transform1->position + startRelativePos0;
		QDEBUG("Setting entity {0} relative position...", entity2Id);
		transform2->position = transform0->position + startRelativePos2;
		QDEBUG("Entity positions: {0} ({1}), {2} ({3}), {4} ({5})",
			entity0Id, OrbitComponent::vec3_tostring(transform0->position,", "),
			entity1Id, OrbitComponent::vec3_tostring(transform1->position, ", "),
			entity2Id, OrbitComponent::vec3_tostring(transform2->position, ", ")
		);


		// Set orbit periods.
		QDEBUG("Setting orbit periods...");
		set_orbit_period(entity0Id, 10.f);
		QDEBUG("Entity {0} new orbit period: {1}", entity0Id, orbit0->mOrbitPeriod);
		set_orbit_period(entity2Id, 0.f);
		QDEBUG("Entity {0} new orbit period: {1}", entity2Id, orbit2->mOrbitPeriod);


		// Set orbit normal.
		QDEBUG("Setting orbit normal...");
		set_orbit_normal(entity0Id, glm::vec3(1,2,-1));
		QDEBUG("Entity {0} new orbit normal: ({1} {2} {3})", entity0Id, orbit0->mAxisNormal.x, orbit0->mAxisNormal.y, orbit0->mAxisNormal.z);


		completeTimesCounter = 30;
		orbitTrackers.emplace(std::make_pair(entity0Id, OrbitTracker()));
		orbitTrackers.emplace(std::make_pair(entity2Id, OrbitTracker()));


		// Deactivate orbit.
		QDEBUG("Clearing orbit {0}...", entity0Id);
		clear_orbit(entity0Id);


		//// Render resources //
		//auto path = FileModule::Instance()->get_internal_assets_path();

		//// Create sphere meshes.
		//auto spherePath = path + "DefaultObjects/sphere20x20.obj";
		//// 0
		//QDEBUG("Creating sphere meshes...");
		//active_manager->create_component<Engine::MeshComponent>(entity0Id, COMPONENT_MESH);
		//MeshComponent* mesh0 = active_manager->get_component<MeshComponent>(entity0Id, COMPONENT_MESH);
		//if (mesh0 == nullptr) {
		//	QDEBUG("Failed to create/retrieve mesh component for entity: {0}", entity0Id);
		//	return;
		//}
		//mesh0->path = spherePath;
		//QDEBUG("Sphere mesh created for entity {0}", entity0Id);
		//// 1
		//active_manager->create_component<Engine::MeshComponent>(entity1Id, COMPONENT_MESH);
		//MeshComponent* mesh1 = active_manager->get_component<MeshComponent>(entity1Id, COMPONENT_MESH);
		//if (mesh1 == nullptr) {
		//	QDEBUG("Failed to create/retrieve mesh component for entity: {0}", entity1Id);
		//	return;
		//}
		//mesh1->path = spherePath;
		//QDEBUG("Sphere mesh created for entity {0}", entity1Id);
		//// 2
		//active_manager->create_component<Engine::MeshComponent>(entity2Id, COMPONENT_MESH);
		//MeshComponent* mesh2 = active_manager->get_component<MeshComponent>(entity2Id, COMPONENT_MESH);
		//if (mesh0 == nullptr) {
		//	QDEBUG("Failed to create/retrieve mesh component for entity: {0}", entity2Id);
		//	return;
		//}
		//mesh2->path = spherePath;
		//QDEBUG("Sphere mesh created for entity {0}", entity2Id);


		//// Create sphere materials.
		//std::string vshPath = path + "Shader/DefaultShader.vsh";
		//std::string fshPath = path + "Shader/DefaultShader.fsh";
		//std::string gshPth = "";
		//std::string texturePath = path + "Texture/floor.jpg";
		//Material* material = new Engine::Material(vshPath, fshPath, gshPth, texturePath);
		//// 0
		//active_manager->create_component<Engine::MaterialComponent>(entity0Id, COMPONENT_MATERIAL);
		//MaterialComponent* mat0 = active_manager->get_component<MaterialComponent>(entity0Id, COMPONENT_MATERIAL);
		//if (mat0 == nullptr) {
		//	QDEBUG("Failed to create/retrieve material component for entity: {0}", entity0Id);
		//	return;
		//}
		//mat0->material = material;
		//// 1
		//active_manager->create_component<Engine::MaterialComponent>(entity1Id, COMPONENT_MATERIAL);
		//MaterialComponent* mat1 = active_manager->get_component<MaterialComponent>(entity1Id, COMPONENT_MATERIAL);
		//if (mat1 == nullptr) {
		//	QDEBUG("Failed to create/retrieve material component for entity: {0}", entity1Id);
		//	return;
		//}
		//mat1->material = material;
		//// 2
		//active_manager->create_component<Engine::MaterialComponent>(entity2Id, COMPONENT_MATERIAL);
		//MaterialComponent* mat2 = active_manager->get_component<MaterialComponent>(entity2Id, COMPONENT_MATERIAL);
		//if (mat2 == nullptr) {
		//	QDEBUG("Failed to create/retrieve material component for entity: {0}", entity2Id);
		//	return;
		//}
		//mat2->material = material;


	} // component_tests()


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