#include "PhysicsManager.h"
#include "LogModule.h"


// singleton
PhysicsManager* PhysicsManager::instance = nullptr;

/// <summary>
/// Create a singleton of the physics manager.
/// </summary>
/// <returns> The unique instance of the physics manager. </returns>
PhysicsManager* PhysicsManager::Instance()
{
	if (nullptr == instance)
		return new PhysicsManager();
	else
		return instance;
}


void PhysicsManager::init()
{
	QDEBUG( "Test for PhysicsManager init()" );


	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher( collisionConfiguration );

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	// Collision world stores all collision objects and provides interface for queries.
	collisionWorld = new btCollisionWorld( dispatcher, overlappingPairCache, collisionConfiguration );



	// Test collision detection //

	btCollisionObject* testCollisionObject = new btCollisionObject();

	// Set object's collision shape.
	btCollisionShape* testCollisionShape = new btSphereShape( 2.66666f ); // Test with sphere shape: radius = 1.
	collisionShapes.push_back( testCollisionShape ); // Store collision shape - can be re-used!
	testCollisionObject->setCollisionShape( testCollisionShape );

	// Set object's world transform.
	float yaw, pitch, roll;
	yaw = 15.f;
	pitch = -99.99999f;
	roll = 0;
	btQuaternion orn = btQuaternion( yaw * SIMD_RADS_PER_DEG, pitch * SIMD_RADS_PER_DEG, roll * SIMD_RADS_PER_DEG ); // Constructing from Euler angles (yawZ, pitchY, rollX).
	btVector3 pos = btVector3( 60.5f, -109.77777f, 0 );
	btTransform testTransform = btTransform( orn, pos );
	testCollisionObject->setWorldTransform( testTransform ); // Apply transform to object.

	// Add test object to the collision world.
	collisionWorld->addCollisionObject( testCollisionObject );

}

int PhysicsManager::start()
{
	QDEBUG( "Tests in PhysicsManager start():" );

	char msg[128];

	QDEBUG( "------------------------------" );
	snprintf( msg, 128, "Number of collision objects in collision world: %d", collisionWorld->getNumCollisionObjects() );
	QDEBUG( msg );


	// Check persistence of collision world state since initialisation.
	
	for ( int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i )
	{

		snprintf( msg, 128, "Collision object %i", i );
		QDEBUG( msg );

		btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];

		btTransform trf = obj->getWorldTransform();

		btQuaternion orn = trf.getRotation();
		float yaw, pitch, roll;
		orn.getEulerZYX(yaw, pitch, roll);
		snprintf( msg, 128, "World Orientation (rad): (%5.5f, %5.5f, %5.5f)", yaw, pitch, roll );
		QDEBUG( msg );
		snprintf( msg, 128, "World Orientation (deg): (%5.5f, %5.5f, %5.5f)", yaw * SIMD_DEGS_PER_RAD, pitch * SIMD_DEGS_PER_RAD, roll * SIMD_DEGS_PER_RAD );
		QDEBUG( msg );

		btVector3 pos = trf.getOrigin();
		snprintf( msg, 128, "World Position: (%5.5f, %5.5f, %5.5f)", pos.getX(), pos.getY(), pos.getZ() );
		QDEBUG( msg );

		btSphereShape* collisionSphere = dynamic_cast<btSphereShape*>( obj->getCollisionShape() );
		if ( nullptr != collisionSphere )
		{
			snprintf( msg, 128, "Collision shape: Sphere, radius: %5.5f", collisionSphere->getRadius() );
			QDEBUG( msg );
		}

	}


	// Test raycasting against collision objects.
	QDEBUG( "------------------------------" ); 
	QDEBUG( "Static raycasting tests, closest hits:" );

	// Raycast from world origin to each collision object origin.
	btVector3 worldOrigin(0,0,0);
	for ( int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i )
	{
		btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];
		btVector3 pos = obj->getWorldTransform().getOrigin();

		// Set up raycast result callback for closest hit.
		btCollisionWorld::ClosestRayResultCallback closestRaycastResult( worldOrigin, pos );
		// closestRaycastResult.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
		// kF_UseGjkConvexRaytest flag is enabled by default
		// allRaycastResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest; // faster than default algorithm but more approximate

		// Cast ray.
		collisionWorld->rayTest( worldOrigin, pos, closestRaycastResult );

		// Print results.
		if ( closestRaycastResult.hasHit() )
		{
			btVector3 hitPoint = closestRaycastResult.m_hitPointWorld;
			snprintf(msg, 128, "Collision object %i hit at world position: (%5.5f, %5.5f, %5.5f)", i, hitPoint.getX(), hitPoint.getY(), hitPoint.getZ() );
			QDEBUG( msg );

			btSphereShape* collisionSphere = dynamic_cast<btSphereShape*>(obj->getCollisionShape());
			if (nullptr != collisionSphere)
			{
				float objClosestPoint = pos.length() - collisionSphere->getRadius();
				snprintf( msg, 128, "Object closest point: %f, hit distance: %f, difference: %f", objClosestPoint, hitPoint.length(), objClosestPoint - hitPoint.length());
				QDEBUG( msg );
			}
		}
		else
		{
			QDEBUG( "Collision object %i not hit" );
		}
	}

	return 0;
}

void PhysicsManager::update()
{

}

int PhysicsManager::stop()
{
	return 0;
}

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
}