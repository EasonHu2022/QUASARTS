#include "PhysicsSystem.h"

#include <sstream>
#include <iomanip>
#include <fstream>


#include "ECS/System/OrbitSystem.h"	// Orbit tests


namespace Engine {

	// singleton
	PhysicsSystem* PhysicsSystem::instance = nullptr;

	/// <summary>
	/// Create a singleton of the physics manager.
	/// </summary>
	/// <returns> The unique instance of the physics manager. </returns>
	PhysicsSystem* PhysicsSystem::Instance()
	{
		if (nullptr == instance)
			instance = new PhysicsSystem();
		return instance;
	}


	void PhysicsSystem::init()
	{
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		// Collision world stores all collision objects and provides interface for queries.
		collisionWorld = new btCollisionWorld(dispatcher, overlappingPairCache, collisionConfiguration);

		setup_collision_world();

		// Test collision world.
		//runTests_init();


	} // init()

	int PhysicsSystem::start()
	{
		// Test collision world.
		//runTests_start();

		return 0;

	} // start()

	void PhysicsSystem::update()
	{
		// Get all overlaps in this frame.
		collisionWorld->performDiscreteCollisionDetection();
		int numManifolds = dispatcher->getNumManifolds();

		btPersistentManifold* contactManifold;
		btCollisionObject* obA, * obB;

		int numContacts;
		int objId0, objId1;
		CollisionObjectInfo* objInfo0, * objInfo1;

		// Create exactly 1 collision event for each unique pair of colliding objects:
		// Iterate over potential collisions.
		for (int i = 0; i < numManifolds; ++i)
		{
			contactManifold = dispatcher->getManifoldByIndexInternal(i);
			obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
			obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

			// Test contact points for a collision.
			numContacts = (contactManifold->getNumContacts() > 0) ? 1 : 0;
			for (int j = 0; j < numContacts; j++)
			{
				btManifoldPoint& pt = contactManifold->getContactPoint(0);
				if (pt.getDistance() < 0.f)
				{
					// Get the colliding entities' information.
					objId0 = get_object_index(obA);
					objInfo0 = &collisionObjectArrayInfo[objId0];

					objId1 = get_object_index(obB);
					objInfo1 = &collisionObjectArrayInfo[objId1];

					// Create the event.
					EventModule::Instance()->create_event(
						"Collision", EventModule::EventPriority::Medium,
						{
							{ "entity0", EV_ARG_INT(objInfo0->mEntityId) },	// while entities are limited to 1 of each type of component, entity ID = component ID
							{ "componentType0", EV_ARG_INT(objInfo0->mUsage) },

							{ "entity1", EV_ARG_INT(objInfo1->mEntityId) },
							{ "componentType1", EV_ARG_INT(objInfo1->mUsage) },	// while entities are limited to 1 of each type of component, entity ID = component ID
						}
					);

					break; // Only create an event for the first collision of this pair.
				}
			}
		}


		// Time tests.
		//time_tests();

	} // update()

	int PhysicsSystem::stop()
	{
		return 0;

	} // stop()

	void PhysicsSystem::release()
	{
		clear_collision_world();

		// Delete collision world objects in reverse order of creation.
		delete collisionWorld;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfiguration;

	} // release()


	// Usage //

	void PhysicsSystem::setup_collision_world()
	{
		clear_collision_world();

		// Create default collision shape.
		collisionSpheres.push_back(new btSphereShape(Q_DEFAULT_SPHERE_RADIUS));

		// Pre-create the default number of collision objects.
		{
			int temp[Q_DEFAULT_NUM_COLLISION_OBJS];
			for (int i = 0; i < Q_DEFAULT_NUM_COLLISION_OBJS; ++i)
			{
				temp[i] = assign_collision_sphere(i, glm::vec3(0, 0, 0), Q_DEFAULT_SPHERE_RADIUS);
			}
			for (int i = 0; i < Q_DEFAULT_NUM_COLLISION_OBJS; ++i)
			{
				unassign_collision_object(temp[i]);
			}
		}

	} // setup_collision_world()


	int PhysicsSystem::assign_collision_sphere(const int aComponentId, const glm::vec3 worldPosition, const float radius)
	{
		// Find an unassigned collision object.
		int obj_idx = get_unassigned_collision_object_index();
		if (obj_idx != -1)
		{
			++numAssignedObjects;

			// Set bookkeeping data.
			CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[obj_idx];
			objInfo->mEntityId = aComponentId;
			objInfo->mUsage = Sphere;

			// Get and set its collision shape.
			btSphereShape* sphere = get_sphere_shape(radius);
			objInfo->pObject->setCollisionShape(sphere);

			// Activate collision detection.
			objInfo->pObject->getBroadphaseHandle()->m_collisionFilterGroup = Q_COLLISION_FILTER_VISIBLE;
			objInfo->pObject->getBroadphaseHandle()->m_collisionFilterMask = Q_COLLISION_FILTER_VISIBLE;
			collisionWorld->refreshBroadphaseProxy(objInfo->pObject);

			// Set its world transform.
			float yaw = 0.f, pitch = 0.f, roll = 0.f;
			btQuaternion orn = btQuaternion(yaw * SIMD_RADS_PER_DEG, pitch * SIMD_RADS_PER_DEG, roll * SIMD_RADS_PER_DEG); // Constructing from Euler angles (yawZ, pitchY, rollX).
			btVector3 pos = glmvec3_to_bt(worldPosition);
			objInfo->pObject->setWorldTransform(btTransform(orn, pos));
		}
		return obj_idx;

	} // assign_collision_sphere()


	void PhysicsSystem::unassign_collision_object(const int aCollisionObjectId)
	{
		if (collisionObjectArrayInfo[aCollisionObjectId].mUsage == Unassigned)
		{
			QERROR("unassign_collision_object() was passed the index of an unassigned collision object: {0}", aCollisionObjectId);
			return;
		}
		--numAssignedObjects;

		// Clear bookkeeping.
		CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[aCollisionObjectId];
		objInfo->mEntityId = -1;
		objInfo->mUsage = Unassigned;

		// Deactivate collision detection.
		objInfo->pObject->getBroadphaseHandle()->m_collisionFilterGroup = Q_COLLISION_FILTER_HIDDEN;
		objInfo->pObject->getBroadphaseHandle()->m_collisionFilterMask = Q_COLLISION_FILTER_HIDDEN;
		collisionWorld->refreshBroadphaseProxy(objInfo->pObject);

	} // unassign_collision_object()


	void PhysicsSystem::set_collision_object_position(const int aObjId, const glm::vec3 aNewWorldPosition)
	{
		CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[aObjId];
		if (objInfo->mUsage == Unassigned)
		{
			QERROR("move_collision_object() was passed the index of an unassigned collision object: {0}", object_to_string(objInfo->pObject));
			return;
		}
		collisionWorld->getCollisionObjectArray()[aObjId]->getWorldTransform().setOrigin(glmvec3_to_bt(aNewWorldPosition));

	} // move_collision_object()


	void PhysicsSystem::set_collision_sphere_radius(int const aObjId, float const aNewRadius)
	{
		CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[aObjId];
		if (objInfo->mUsage != Sphere)
		{
			QERROR("PhysicsSystem::set_collision_sphere_radius(): Received ID of non-sphere collision object: {0}", object_to_string(objInfo->pObject));
			return;
		}
		// Get and set new collision sphere shape (previous shape is not discarded).
		btSphereShape* sphere = get_sphere_shape(aNewRadius);
		objInfo->pObject->setCollisionShape(sphere);

	} // set_collision_sphere_radius()


	bool PhysicsSystem::raycast(const glm::vec3 origin, const glm::vec3 direction, glm::vec3* hitLocation)
	{
		btVector3 hitLoc;
		bool ret = raycast_bt(glmvec3_to_bt(origin), glmvec3_to_bt(direction), &hitLoc);
		*hitLocation = btvector3_to_glm(hitLoc);
		return ret;

	} // raycast()


	// Util //

	void PhysicsSystem::clear_collision_world()
	{
		// Remove pointers to collision objects.
		for (int i = 0; i < Q_MAX_COLLISION_OBJS; ++i)
		{
			collisionObjectArrayInfo[i].pObject = nullptr;
		}

		// Delete collision objects in reverse order of creation.
		for (int i = collisionWorld->getNumCollisionObjects() - 1; i >= 0; --i)
		{
			btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
			collisionWorld->removeCollisionObject(obj);
			delete obj;
		}

		// Delete collision spheres (no longer referenced by collision objects).
		for (int i = 0; i < collisionSpheres.size(); ++i)
		{
			btSphereShape* sphere = collisionSpheres[i];
			collisionSpheres[i] = nullptr; // Remove pointer from array.
			delete sphere;
		}
		collisionSpheres.clear();

	} // reset_collision_world()


	int PhysicsSystem::get_object_index(const btCollisionObject* obj)
	{
		if (obj == nullptr)
		{
			QERROR("get_object_index() was passed a null pointer");
			return -1;
		}
		for (int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i)
		{
			if (obj == collisionWorld->getCollisionObjectArray()[i])
				return i;
		}
		return -1;

	} // get_object_index()


	int PhysicsSystem::get_unassigned_collision_object_index()
	{
		// Return the index of the first available object.
		int numObjs = collisionWorld->getNumCollisionObjects();
		for (int i = 0; i < numObjs; ++i)
		{
			if (collisionObjectArrayInfo[i].mUsage == Unassigned)
			{
				return i;
			}
		}
		// If none are available and there are fewer than the maximum, create new and return its index.
		if (numObjs < Q_MAX_COLLISION_OBJS)
		{
			CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[numObjs];
			objInfo->pObject = new btCollisionObject();
			objInfo->pObject->setCollisionShape(get_sphere_shape(Q_DEFAULT_SPHERE_RADIUS));
			collisionWorld->addCollisionObject(objInfo->pObject, Q_COLLISION_FILTER_HIDDEN, Q_COLLISION_FILTER_HIDDEN);
			return numObjs;
		}
		QDEBUG("PhysicsSystem: get_unassigned_collision_object_index(): too many collision objects, could not assign new.");
		return -1;

	} // get_available_collision_object()


	btSphereShape* PhysicsSystem::get_sphere_shape(float radius)
	{
		// Find an existing btSphereShape with (approximately) the required radius.
		size_t numSpheres = collisionSpheres.size();
		for (int i = 0; i < numSpheres; ++i)
		{
			if (abs(collisionSpheres[i]->getRadius() - radius) < Q_COLLISION_EPSILON)
			{
				return collisionSpheres[i];
			}
		}

		// If none was found, create new.
		++numSpheres;
		if (numSpheres > Q_MAX_COLLISION_SPHERE_SHAPES)
		{
			QDEBUG("PhysicsSystem::get_sphere_shape(): btSphereShape created; maximum number ({0}) of btSphereShapes exceeded. New number: {1}", Q_MAX_COLLISION_SPHERE_SHAPES, numSpheres);
		}
		btSphereShape* sphere = new btSphereShape(radius);
		collisionSpheres.push_back(sphere);
		return sphere;

	} // getSphereShape()


	bool PhysicsSystem::raycast_bt(const btVector3 origin, const btVector3 direction, btVector3* hitLocation)
	{
		btVector3 end = origin + direction;
		if (direction.length() < Q_RAYCAST_RAY_MIN_LENGTH)
			end = origin + (direction.normalized() * Q_RAYCAST_RAY_MIN_LENGTH);

		btCollisionWorld::ClosestRayResultCallback closestRaycastResult(origin, end);
		closestRaycastResult.m_collisionFilterGroup = Q_COLLISION_FILTER_VISIBLE;
		closestRaycastResult.m_collisionFilterMask = Q_COLLISION_FILTER_VISIBLE;

		collisionWorld->rayTest(origin, end, closestRaycastResult);
		if (closestRaycastResult.hasHit())
		{
			*hitLocation = closestRaycastResult.m_hitPointWorld;
			return true;
		}
		return false;

	} // raycast_bt()


	// Debug //

	std::string PhysicsSystem::object_to_string(const btCollisionObject* obj, const bool angles_to_deg)
	{
		std::ostringstream ostr;
		ostr << "transform: " << transform_to_string(&obj->getWorldTransform(), angles_to_deg);
		ostr << ", shape: " << shape_to_string(obj->getCollisionShape());
		ostr << ", collision mask: " << obj->getBroadphaseHandle()->m_collisionFilterMask;
		ostr << ", collision group: " << obj->getBroadphaseHandle()->m_collisionFilterGroup;
		return ostr.str();
	} // object_to_string()

	std::string PhysicsSystem::transform_to_string(const btTransform* trf, const bool angles_to_deg)
	{
		std::ostringstream ostr;

		ostr << std::setprecision(3);

		// Position
		btVector3 pos = trf->getOrigin();
		ostr << "position: " << vector_to_string(pos);

		// Orientation
		btQuaternion orn = trf->getRotation();
		float yaw, pitch, roll;
		orn.getEulerZYX(yaw, pitch, roll);
		if (angles_to_deg)
		{
			yaw *= SIMD_DEGS_PER_RAD;
			pitch *= SIMD_DEGS_PER_RAD;
			roll *= SIMD_DEGS_PER_RAD;
		}
		ostr << ", orientation: (" << yaw << ", " << pitch << ", " << roll << ")";

		return ostr.str();
	} // transform_to_string()

	std::string PhysicsSystem::shape_to_string(const btCollisionShape* shape)
	{
		std::ostringstream ostr;

		ostr << std::setprecision(3);

		switch (shape->getShapeType())
		{
		case (SPHERE_SHAPE_PROXYTYPE):
			ostr << "Sphere, radius: " << ((btSphereShape*)shape)->getRadius();
			break;
		}

		return ostr.str();
	} // shape_to_string()

	std::string PhysicsSystem::vector_to_string(const btVector3 vec)
	{
		std::ostringstream ostr;
		ostr << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
		return ostr.str();
	} // vector_to_string()

	std::string PhysicsSystem::object_tostring(int const objId)
	{
		return object_to_string(collisionObjectArrayInfo[objId].pObject);
	} // object_tostring()







	// Tests //

	void PhysicsSystem::time_tests()
	{
		QTime deltaT = TimeModule::Instance()->get_frame_delta_time();
		if (timeCounter.sec() < 0)
		{
			QDEBUG("engine time: {0}, deltaT: {1}, FPS: {2}",
				TimeModule::Instance()->get_time().sec(),
				deltaT.sec(),
				(1.f / deltaT.sec())
			);
			timeCounter += 1;
		}
		timeCounter -= deltaT.sec();

	} // time_tests()


	void PhysicsSystem::runTests_init()
	{
		//pretendComponents.push_back( assign_collision_sphere(pretendComponents.size(), glm::vec3(10,0,0), 5.f) );		// component ID 0
		//pretendComponents.push_back( assign_collision_sphere(pretendComponents.size(), glm::vec3(20,0,0), 5.1f) );		// component ID 1
		//pretendComponents.push_back( assign_collision_sphere(pretendComponents.size(), glm::vec3(0,0,-25), 5.f) );		// component ID 2
		// target overlapping pairs: {0,1}, 

		for (int i = 0; i < 128; ++i)
		{
			pretendComponents.push_back(assign_collision_sphere(pretendComponents.size(), glm::vec3(i + 1, 0, 0), 0.1f));
		}

		char msg[512];

		QDEBUG("------------------------------");
		snprintf(msg, 512, "No. collision spheres : %d", collisionSpheres.size());
		QDEBUG(msg);
		snprintf(msg, 512, "No. collision objects in collision world: %d", collisionWorld->getNumCollisionObjects());
		QDEBUG(msg);
		snprintf(msg, 512, "Assigned object/component pairs: %d", numAssignedObjects);
		QDEBUG(msg);
		for (int i = 0; i < Q_MAX_COLLISION_OBJS; ++i)
		{
			CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[i];
			if (objInfo->mUsage == Unassigned) continue;
			snprintf(msg, 512, "- component: %d, object: %d, %s", objInfo->mEntityId, i, object_to_string(objInfo->pObject).c_str());
			QDEBUG(msg);
		}


		// Test getting object by pointer

		for (int i = 0; i < Q_MAX_COLLISION_OBJS; ++i)
		{
			auto* objInfo = &collisionObjectArrayInfo[i];
			if (objInfo->mUsage != Unassigned)
				assert(i == get_object_index(objInfo->pObject));
		}


		// Test raytrace filtering

		/*QDEBUG("------------------------------");
		QDEBUG("Test raycasting:");
		for (int i = 0; i < Q_MAX_COLLISION_OBJS; ++i)
		{
			CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[i];
			if (objInfo->mUsage == Unassigned && objInfo->pObject != nullptr)
			{
				snprintf(msg, 512, "Control object (Unassigned): %s", object_to_string(objInfo->pObject).c_str());
				QDEBUG(msg);
				break;
			}
		}

		btVector3 rayOrg;
		btVector3 rayDir;
		btVector3 hitLoc;
		bool hitRet;

		QDEBUG("Raycast 1:");
		rayOrg.setValue(-10, 0, 0);
		rayDir.setValue(  1, 0, 0);
		hitRet = raycast_bt(rayOrg, rayDir, &hitLoc);
		snprintf(msg, 512, "- origin: %s, direction: %s, hit: %s, location: %s",
			vector_to_string(rayOrg).c_str(),
			vector_to_string(rayDir).c_str(),
			(hitRet ? "T" : "F"),
			(hitRet ? vector_to_string(hitLoc).c_str() : "n/a"));
		QDEBUG(msg);

		QDEBUG("Raycast 2:");
		rayOrg.setValue( 20, 0, 0);
		rayDir.setValue( -1, 0, 0);
		hitRet = raycast_bt(rayOrg, rayDir, &hitLoc);
		snprintf(msg, 512, "- origin: %s, direction: %s, hit: %s, location: %s",
			vector_to_string(rayOrg).c_str(),
			vector_to_string(rayDir).c_str(),
			(hitRet ? "T" : "F"),
			(hitRet ? vector_to_string(hitLoc).c_str() : "n/a"));
		QDEBUG(msg);*/

		// Test overlapping pairs

		/*collisionWorld->performDiscreteCollisionDetection();
		btOverlappingPairCache* pairs = overlappingPairCache->getOverlappingPairCache();

		QDEBUG("------------------------------");
		snprintf(msg, 512, "Overlapping pairs: %d", pairs->getNumOverlappingPairs());
		QDEBUG(msg);
		for (int i = 0; i < pairs->getNumOverlappingPairs(); ++i)
		{
			snprintf(msg, 512, "- pair %d:", i);
			QDEBUG(msg);
			auto pair = pairs->getOverlappingPairArray()[i];
			btCollisionObject* obj = (btCollisionObject*)pair.m_pProxy0->m_clientObject;
			snprintf(msg, 512, "  - object 1: %s", object_to_string(obj, true).c_str());
			QDEBUG(msg);
			obj = (btCollisionObject*)pair.m_pProxy1->m_clientObject;
			snprintf(msg, 512, "  - object 2: %s", object_to_string(obj, true).c_str());
			QDEBUG(msg);
		}


		QDEBUG("------------------------------");
		QDEBUG("Unassign testObjIds: idx 0");
		unassign_collision_object(testObjIds[0]);
		testObjIds[0] = -1;
		snprintf(msg, 512, "Test objects: %d", testObjIds.size());
		QDEBUG(msg);
		for (int i = 0; i < testObjIds.size(); ++i)
		{
			snprintf(msg, 512, "- object %d: index %d", i, testObjIds[i]);
			QDEBUG(msg);
		}
		snprintf(msg, 512, "Number of assigned objects: %d", numAssignedObjects);
		QDEBUG(msg);
		for (int i = 0; i < Q_MAX_COLLISION_OBJS; ++i)
		{
			CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[i];
			if (objInfo->mUsage == Unassigned) continue;
			snprintf(msg, 512, "- assigned object ID: %d, component ID: %d", i, objInfo->mComponentId);
			QDEBUG(msg);
		}
		snprintf(msg, 512, "Number of collision spheres : %d", collisionSpheres.size());
		QDEBUG(msg);
		snprintf(msg, 512, "Collision objects in collision world: %d", collisionWorld->getNumCollisionObjects());
		QDEBUG(msg);
		for (int i = 0; i < 3; ++i)
		{
			btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
			snprintf(msg, 512, "- object %d: %s", i, object_to_string(obj, true).c_str());
			QDEBUG(msg);
		}

		collisionWorld->performDiscreteCollisionDetection();
		pairs = overlappingPairCache->getOverlappingPairCache();
		QDEBUG("------------------------------");
		snprintf(msg, 512, "Overlapping pairs: %d", pairs->getNumOverlappingPairs());
		QDEBUG(msg);
		for (int i = 0; i < pairs->getNumOverlappingPairs(); ++i)
		{
			snprintf(msg, 512, "- pair %d:", i);
			QDEBUG(msg);
			auto pair = pairs->getOverlappingPairArray()[i];
			btCollisionObject* obj = (btCollisionObject*)pair.m_pProxy0->m_clientObject;
			snprintf(msg, 512, "  - object 1: %s", object_to_string(obj, true).c_str());
			QDEBUG(msg);
			obj = (btCollisionObject*)pair.m_pProxy1->m_clientObject;
			snprintf(msg, 512, "  - object 2: %s", object_to_string(obj, true).c_str());
			QDEBUG(msg);
		}


		QDEBUG("------------------------------");
		QDEBUG("Assign testObjIds: 3, Re-assign testObjIds: idx 0");
		testObjIds.push_back( assign_collision_sphere( testObjIds.size(), glm::vec3(-0.5, -0.5, 0), 3.f) );
		testObjIds[0] = assign_collision_sphere(0, glm::vec3(0, 51, 0), 5.f);
		snprintf(msg, 512, "Test objects: %d", testObjIds.size());
		QDEBUG(msg);
		for (int i = 0; i < testObjIds.size(); ++i)
		{
			snprintf(msg, 512, "- object %d: index %d", i, testObjIds[i]);
			QDEBUG(msg);
		}
		snprintf(msg, 512, "Number of assigned objects: %d", numAssignedObjects);
		QDEBUG(msg);
		for (int i = 0; i < Q_MAX_COLLISION_OBJS; ++i)
		{
			CollisionObjectInfo* objInfo = &collisionObjectArrayInfo[i];
			if (objInfo->mUsage == Unassigned) continue;
			snprintf(msg, 512, "- assigned object ID: %d, component ID: %d", i, objInfo->mComponentId);
			QDEBUG(msg);
		}
		snprintf(msg, 512, "Number of collision spheres : %d", collisionSpheres.size());
		QDEBUG(msg);
		snprintf(msg, 512, "Collision objects in collision world: %d", collisionWorld->getNumCollisionObjects());
		QDEBUG(msg);
		for (int i = 0; i < 3; ++i)
		{
			btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
			snprintf(msg, 512, "- object %d: %s", i, object_to_string(obj, true).c_str());
			QDEBUG(msg);
		}


		collisionWorld->performDiscreteCollisionDetection();
		pairs = overlappingPairCache->getOverlappingPairCache();
		QDEBUG("------------------------------");
		snprintf(msg, 512, "Overlapping pairs: %d", pairs->getNumOverlappingPairs());
		QDEBUG(msg);
		for (int i = 0; i < pairs->getNumOverlappingPairs(); ++i)
		{
			snprintf(msg, 512, "- pair %d:", i);
			QDEBUG(msg);
			auto pair = pairs->getOverlappingPairArray()[i];
			btCollisionObject* obj = (btCollisionObject*)pair.m_pProxy0->m_clientObject;
			snprintf(msg, 512, "  - object 1: %s", object_to_string(obj, true).c_str());
			QDEBUG(msg);
			obj = (btCollisionObject*)pair.m_pProxy1->m_clientObject;
			snprintf(msg, 512, "  - object 2: %s", object_to_string(obj, true).c_str());
			QDEBUG(msg);
		}*/


	} // runTests_init()

	void PhysicsSystem::runTests_start()
	{
		// Event tests //
		/*
		// Test event handler registration.
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(DebugEvent));

		// Test event submission.
		{
			// Events submitted in reverse order of priority to demonstrate the queue's ability to sort by priority level.

			// Event 0
			EventModule::Instance()->create_event( "DebugEvent", EventModule::EventPriority::High, { {"distance", EV_ARG_FLOAT(3.7e4)} });

			// Event 1
			EventModule::Instance()->create_event( "DebugEvent", EventModule::EventPriority::Medium, { {"code", EV_ARG_INT(39)} } );

			// Event 2
			EventModule::Instance()->create_event( "DebugEvent", EventModule::EventPriority::Low, { {"code", EV_ARG_INT(17)}, {"repeat", EV_ARG_BOOL(true)} } );

			// Event 3
			EventModule::Instance()->create_event( "DebugEvent", EventModule::EventPriority::High, { {"name", EV_ARG_STRING("Jim")} } );
		}
		EventModule::Instance()->log_queue();
		*/


		// Object tests //
		/*
		char msg[512];
		QDEBUG("------------------------------");
		snprintf(msg, 512, "Number of collision spheres : %d", collisionSpheres.size());
		QDEBUG(msg);
		snprintf(msg, 512, "Number of collision objects in collision world: %d", collisionWorld->getNumCollisionObjects());
		QDEBUG(msg);

		// Check persistence of collision world state since initialisation.

		for (int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i)
		{
			btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];

			snprintf(msg, 512, "- object %i: %s", i, object_to_string(obj, true).c_str());
			QDEBUG(msg);
		}

		// Collision tests //

		btVector3 hitLoc;
		btVector3 rayOrigin = btVector3(10, 0, 0);
		btVector3 rayDirection = btVector3(-10, 0, 0);
		bool ret = raycast_bt(rayOrigin, rayDirection, &hitLoc);
		snprintf(msg, 512, "Hit test, returned: %s, %s", (ret ? "true" : "false"), (ret ? vector_to_string(hitLoc).c_str() : ""));
		QDEBUG(msg);
		*/

		/*
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


	void PhysicsSystem::EV_CALLBACK_SIGNATURE(DebugEvent)
	{
		char msg[512];
		snprintf(msg, 512, "PhysicsSystem handler for type 'DebugEvent' called, received Event:\n%s", evt.to_string().c_str());
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

		name = "distance";
		float flarg = 0.f;
		ret = evt.find_argument(&flarg, name);
		snprintf(msg, 512, "Event argument query for '%s' returned: '%s', value:", name.c_str(), (ret ? "true" : "false"));
		QDEBUG(msg);
		if (ret) {
			snprintf(msg, 512, "Event argument value: '%f'", flarg);
			QDEBUG(msg);
		}
	}

}