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
	btCollisionShape* testCollisionShape = new btSphereShape( 1.f ); // Test with sphere shape: radius = 1.
	collisionShapes.push_back( testCollisionShape ); // Store collision shape - can be re-used!
	testCollisionObject->setCollisionShape( testCollisionShape );

	// Set object's world transform.
	btQuaternion orn = btQuaternion( 15.f, -99.99999f, 0 ); // Constructing from Euler angles (yawZ, pitchY, rollX).
	btVector3 pos = btVector3( 1.33333f, -3.f, 0 );
	btTransform testTransform = btTransform( orn, pos );
	testCollisionObject->setWorldTransform( testTransform ); // Apply transform to object.

	// Add test object to the collision world.
	collisionWorld->addCollisionObject( testCollisionObject );

}

int PhysicsManager::start()
{
	QDEBUG("Test for PhysicsManager start():\n");

	char msg[64];
	snprintf(msg, 64, "Number of collision objects in collision world: %d\n", collisionWorld->getNumCollisionObjects());
	QDEBUG(msg);

	// Check persistence of collision world state since initialisation.
	
	for (int i = 0; i < collisionWorld->getNumCollisionObjects(); ++i)
	{

		snprintf( msg, 64, "Collision object %i", i );
		QDEBUG( msg );

		btCollisionObject* obj = collisionWorld->getCollisionObjectArray()[i];

		btTransform trf = obj->getWorldTransform();

		btQuaternion orn = trf.getRotation();
		float yaw, pitch, roll;
		orn.getEulerZYX(yaw, pitch, roll);
		snprintf( msg, 64, "World Orientation: (%5.5f, %5.5f, %5.5f)", yaw, pitch, roll );
		QDEBUG( msg );

		btVector3 pos = trf.getOrigin();
		snprintf( msg, 64, "World Position: (%5.5f, %5.5f, %5.5f)", pos.getX(), pos.getY(), pos.getZ() );
		QDEBUG( msg );
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