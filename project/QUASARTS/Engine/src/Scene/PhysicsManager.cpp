#include "PhysicsManager.h"
#include "Logger/LogModule.h"


// singleton
PhysicsManager* PhysicsManager::instance = nullptr;

/// <summary>
/// Create a singleton of the physics manager.
/// </summary>
/// <returns> The unique instance of the physics manager. </returns>
PhysicsManager* PhysicsManager::Instance()
{
	if (nullptr == instance)
		instance = new PhysicsManager();
	return instance;
}


void PhysicsManager::init()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher( collisionConfiguration );

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	// Collision world stores all collision objects and provides interface for queries.
	collisionWorld = new btCollisionWorld( dispatcher, overlappingPairCache, collisionConfiguration );


	// Test collision world.
	//runTests_init();

}

int PhysicsManager::start()
{

	// Test collision world.
	runTests_start();

	return 0;

} // start()

void PhysicsManager::update()
{

} // update()

int PhysicsManager::stop()
{
	return 0;

} // stop()

void PhysicsManager::release()
{
	// Delete collision objects in reverse order of creation.
	for ( int i = collisionWorld->getNumCollisionObjects() - 1; i >= 0; --i )
	{
		btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
		collisionWorld->removeCollisionObject( obj );
		delete obj;
	}

	// Delete collision shapes (should no longer be dependended on).
	for ( int i = 0; i < collisionShapes.size(); ++i )
	{
		btCollisionShape* shape = collisionShapes[i];
		collisionShapes[i] = 0; // Remove pointer from array.
		delete shape;
	}

	// Delete collision detection objects in reverse order of creation.
	delete collisionWorld;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;

} // release()


void PhysicsManager::runTests_init()
{
	
	// Test collision detection //

	btCollisionObject* testCollisionObject = new btCollisionObject();

	// Set object's collision shape.
	btCollisionShape* testCollisionShape = new btSphereShape(2.66666f); // Test with sphere shape: radius = 1.
	collisionShapes.push_back(testCollisionShape); // Store collision shape - can be re-used!
	testCollisionObject->setCollisionShape(testCollisionShape);

	// Set object's world transform.
	float yaw, pitch, roll;
	yaw = 15.f;
	pitch = -99.99999f;
	roll = 0;
	btQuaternion orn = btQuaternion(yaw * SIMD_RADS_PER_DEG, pitch * SIMD_RADS_PER_DEG, roll * SIMD_RADS_PER_DEG); // Constructing from Euler angles (yawZ, pitchY, rollX).
	btVector3 pos = btVector3(60.5f, -109.77777f, 0);
	btTransform testTransform = btTransform(orn, pos);
	testCollisionObject->setWorldTransform(testTransform); // Apply transform to object.

	// Add test object to the collision world.
	collisionWorld->addCollisionObject(testCollisionObject);

} // runTests_init()

void PhysicsManager::runTests_start()
{

	// Test event submission.
	{
		// Events submitted in reverse order of priority to demonstrate the queue's ability to sort by priority level.

		// Event 0
		//EventModule::Instance()->create_event( "KeyPressed", EventModule::EventPriority::High );

		// Event 1
		//EventModule::Instance()->create_event( "KeyReleased", EventModule::EventPriority::Medium, { {"code", EV_ARG_INT(39)} } );

		// Event 2
		//EventModule::Instance()->create_event( "KeyPressed", EventModule::EventPriority::Low, { {"code", EV_ARG_INT(17)}, {"repeat", EV_ARG_BOOL(true)} } );

		// Event 3
		//EventModule::Instance()->create_event( "KeyReleased", EventModule::EventPriority::High, { {"name", EV_ARG_STRING("Jim")} } );

		// Event 4
		//EventModule::Instance()->create_KeyPressed_event(KeyCode::_0);

		// Event 5
		//EventModule::Instance()->create_KeyPressed_event(KeyCode::_W);

		// Event 6
		//EventModule::Instance()->create_KeyReleased_event(KeyCode::SPACE);

		// Event 7
		EventModule::Instance()->create_event("KeyPressed", EventModule::EventPriority::High, { {"msg", EV_ARG_STRING("1234567890")} });

		// Event 8
		EventModule::Instance()->create_event("KeyPressed", EventModule::EventPriority::Medium, { {"msg", EV_ARG_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF")} });

		// Event 9
		EventModule::Instance()->create_event("KeyPressed", EventModule::EventPriority::Low, { {"msg", EV_ARG_STRING("0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF")} });
	}
	EventModule::Instance()->log_queue();

	// Test event handler registration.
	EventModule::Instance()->register_handler( EV_CALLBACK_REGISTRATION( KeyPressed ) );
	EventModule::Instance()->register_handler( EV_CALLBACK_REGISTRATION( KeyReleased ) );




	/*
	char msg[128];
	QDEBUG("------------------------------");
	snprintf(msg, 128, "Number of collision objects in collision world: %d", collisionWorld->getNumCollisionObjects());
	QDEBUG(msg);


	// Check persistence of collision world state since initialisation.

	for (int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i)
	{

		snprintf(msg, 128, "Collision object %i", i);
		QDEBUG(msg);

		btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];

		btTransform trf = obj->getWorldTransform();

		btQuaternion orn = trf.getRotation();
		float yaw, pitch, roll;
		orn.getEulerZYX(yaw, pitch, roll);
		snprintf(msg, 128, "World Orientation (rad): (%5.5f, %5.5f, %5.5f)", yaw, pitch, roll);
		QDEBUG(msg);
		snprintf(msg, 128, "World Orientation (deg): (%5.5f, %5.5f, %5.5f)", yaw * SIMD_DEGS_PER_RAD, pitch * SIMD_DEGS_PER_RAD, roll * SIMD_DEGS_PER_RAD);
		QDEBUG(msg);

		btVector3 pos = trf.getOrigin();
		snprintf(msg, 128, "World Position: (%5.5f, %5.5f, %5.5f)", pos.getX(), pos.getY(), pos.getZ());
		QDEBUG(msg);

		btSphereShape* collisionSphere = dynamic_cast<btSphereShape*>(obj->getCollisionShape());
		if (nullptr != collisionSphere)
		{
			snprintf(msg, 128, "Collision shape: Sphere, radius: %5.5f", collisionSphere->getRadius());
			QDEBUG(msg);
		}

	}


	// Test raycasting against collision objects.
	QDEBUG("------------------------------");
	QDEBUG("Static raycasting tests, closest hits:");

	// Raycast from world origin to each collision object origin.
	btVector3 worldOrigin(0, 0, 0);
	for (int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i)
	{
		btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
		btVector3 pos = obj->getWorldTransform().getOrigin();

		// Set up raycast result callback for closest hit.
		btCollisionWorld::ClosestRayResultCallback closestRaycastResult(worldOrigin, pos);
		// closestRaycastResult.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
		// kF_UseGjkConvexRaytest flag is enabled by default
		// allRaycastResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest; // faster than default algorithm but more approximate

		// Cast ray.
		collisionWorld->rayTest(worldOrigin, pos, closestRaycastResult);

		// Print results.
		if (closestRaycastResult.hasHit())
		{
			btVector3 hitPoint = closestRaycastResult.m_hitPointWorld;
			snprintf(msg, 128, "Collision object %i hit at world position: (%5.5f, %5.5f, %5.5f)", i, hitPoint.getX(), hitPoint.getY(), hitPoint.getZ());
			QDEBUG(msg);

			btSphereShape* collisionSphere = dynamic_cast<btSphereShape*>(obj->getCollisionShape());
			if (nullptr != collisionSphere)
			{
				float objClosestPoint = pos.length() - collisionSphere->getRadius();
				snprintf(msg, 128, "Object closest point: %f, hit distance: %f, difference: %f", objClosestPoint, hitPoint.length(), objClosestPoint - hitPoint.length());
				QDEBUG(msg);
			}
		}
		else
		{
			QDEBUG("Collision object %i not hit");
		}
	}
	*/

} // runTests_start()


void PhysicsManager::EV_CALLBACK_SIGNATURE( KeyPressed )
{
	char msg[512];
	snprintf(msg, 512, "PhysicsManager handler for type 'KeyPressed' called, received Event:\n%s", evt.to_string().c_str());
	QDEBUG(msg);

	std::string name = "code";
	int iarg = -1;
	bool ret = evt.find_argument(&iarg, name);
	snprintf(msg, 512, "Event argument query for '%s' returned: '%s'", name.c_str(), (ret ? "true" : "false"));
	QDEBUG(msg);
	if (ret) {
		snprintf(msg, 512, "Event argument value: %d", iarg);
		QDEBUG(msg);
	}

	name = "repeat";
	bool barg = false;
	ret = evt.find_argument(&barg, name);
	snprintf(msg, 512, "Event argument query for '%s' returned: '%s'", name.c_str(), (ret ? "true" : "false"));
	QDEBUG(msg);
	if (ret) {
		snprintf(msg, 512, "Event argument value: '%s'", (barg ? "true" : "false"));
		QDEBUG(msg);
	}
}

void PhysicsManager::EV_CALLBACK_SIGNATURE( KeyReleased )
{
	char msg[512];
	snprintf(msg, 512, "PhysicsManager handler for type 'KeyReleased' called, received Event:\n%s", evt.to_string().c_str());
	QDEBUG(msg);

	std::string name = "code";
	int iarg = -1;
	bool ret = evt.find_argument(&iarg, name);
	snprintf(msg, 512, "Event argument query for '%s' returned: '%s'", name.c_str(), (ret ? "true" : "false"));
	QDEBUG(msg);
	if (ret) {
		snprintf(msg, 512, "Event argument value: %d", iarg);
		QDEBUG(msg);
	}

	name = "repeat";
	bool barg = false;
	ret = evt.find_argument(&barg, name);
	snprintf(msg, 512, "Event argument query for '%s' returned: '%s'", name.c_str(), (ret ? "true" : "false"));
	QDEBUG(msg);
	if (ret) {
		snprintf(msg, 512, "Event argument value: '%s'", (barg ? "true" : "false"));
		QDEBUG(msg);
	}

	name = "name";
	std::string strarg = "";
	ret = evt.find_argument(&strarg, name);
	snprintf(msg, 512, "Event argument query for '%s' returned: '%s'", name.c_str(), (ret ? "true" : "false"));
	QDEBUG(msg);
	if (ret) {
		snprintf(msg, 512, "Event argument value: '%s'", strarg.c_str());
		QDEBUG(msg);
	}
}