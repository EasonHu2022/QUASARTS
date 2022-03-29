#include "PhysicsSystem.h"
#include "Logger/LogModule.h"

#include <sstream>
#include <iomanip>


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


		// Test collision world.
		runTests_init();

	}

	int PhysicsSystem::start()
	{

		// Test collision world.
		runTests_start();

		return 0;

	} // start()

	void PhysicsSystem::update()
	{

	} // update()

	int PhysicsSystem::stop()
	{
		return 0;

	} // stop()

	void PhysicsSystem::release()
	{
		// Delete collision objects in reverse order of creation.
		for (int i = collisionWorld->getNumCollisionObjects() - 1; i >= 0; --i)
		{
			btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
			collisionWorld->removeCollisionObject(obj);
			delete obj;
		}

		// Delete collision sphere (should no longer be dependended on).
		for (int i = 0; i < collisionSpheres.size(); ++i)
		{
			btSphereShape* sphere = collisionSpheres[i];
			collisionSpheres[i] = 0; // Remove pointer from array.
			delete sphere;
		}

		// Delete collision detection objects in reverse order of creation.
		delete collisionWorld;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfiguration;

	} // release()


	void PhysicsSystem::create_collision_sphere(const float radius)
	{
		btCollisionObject* obj = new btCollisionObject();

		btSphereShape* sphere;
		// Set collision shape to an existing btSphereShape with same radius, if one exists.
		int i = 0;
		for (; i < collisionSpheres.size(); ++i)
		{
			if (abs(collisionSpheres[i]->getRadius() - radius) < Q_COLLISION_EPSILON)
			{
				sphere = collisionSpheres[i];
				break;
			}
		}
		// If none was found, create new.
		if (i == collisionSpheres.size())
		{
			sphere = new btSphereShape(radius);
			collisionSpheres.push_back(sphere);
		}

		// Set object's collision shape.
		obj->setCollisionShape(sphere);

		// Set object's world transform.
		float yaw = 0.f, pitch = 0.f, roll = 0.f;
		btQuaternion orn = btQuaternion(yaw * SIMD_RADS_PER_DEG, pitch * SIMD_RADS_PER_DEG, roll * SIMD_RADS_PER_DEG); // Constructing from Euler angles (yawZ, pitchY, rollX).

		btVector3 pos = btVector3(0.f, 0.f, 0.f);

		btTransform transform = btTransform(orn, pos);
		obj->setWorldTransform(transform); // Apply transform to object.

		// Add test object to the collision world.
		collisionWorld->addCollisionObject(obj);

	}

	bool PhysicsSystem::raycast(const glm::vec3 origin, const glm::vec3 direction, glm::vec3* hitLocation)
	{
		btVector3 hitLoc;
		bool ret = raycast(glm_to_bt_vec3(origin), glm_to_bt_vec3(direction), &hitLoc);
		*hitLocation = bt_to_glm_vec3(hitLoc);
		return ret;
	}


	// Util //

	bool PhysicsSystem::raycast(const btVector3 origin, const btVector3 direction, btVector3* hitLocation)
	{
		btVector3 end = direction;
		if (end.length() < Q_RAYCAST_RAY_MIN_LENGTH)
			end.normalize() *= Q_RAYCAST_RAY_MIN_LENGTH;

		btCollisionWorld::ClosestRayResultCallback closestRaycastResult(origin, end);
		collisionWorld->rayTest(origin, end, closestRaycastResult);

		if (closestRaycastResult.hasHit())
		{
			*hitLocation = closestRaycastResult.m_hitPointWorld;
			return true;
		}
		return false;
	}


	std::string PhysicsSystem::object_to_string(const btCollisionObject* obj, const bool angles_to_deg)
	{
		std::ostringstream ostr;
		ostr << "transform: " << transform_to_string(&obj->getWorldTransform(), angles_to_deg);
		ostr << ", shape: " << shape_to_string(obj->getCollisionShape());
		// TODO : collision shape
		return ostr.str();
	}


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
	}


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
	}


	std::string PhysicsSystem::vector_to_string(const btVector3 vec)
	{
		std::ostringstream ostr;
		ostr << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
		return ostr.str();
	}








	void PhysicsSystem::runTests_init()
	{
		create_collision_sphere(5.f);
		create_collision_sphere(5.1f);
		create_collision_sphere(5.f);

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
		bool ret = raycast(rayOrigin, rayDirection, &hitLoc);
		snprintf(msg, 512, "Hit test, returned: %s, %s", (ret?"true":"false"), (ret?vector_to_string(hitLoc).c_str():""));
		QDEBUG(msg);

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