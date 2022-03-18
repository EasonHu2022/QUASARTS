#pragma once

#include "Core/IManager.h"
#include "btBulletCollisionCommon.h"
#include "Event/EventModule.h"
#include "Core/core.h"

class QS_API PhysicsManager : public IManager
{
// singleton
private:
	static PhysicsManager* instance;
	PhysicsManager() {};
public:
	static PhysicsManager* Instance();
	~PhysicsManager() {};


public:
	void init();
	int start();
	void update();
	int stop();
	void release();


private:
	btDefaultCollisionConfiguration*		collisionConfiguration;
	btCollisionDispatcher*					dispatcher;
	btBroadphaseInterface*					overlappingPairCache;
	btCollisionWorld*						collisionWorld;
	// Store collision shapes.
	// Re-use collision shapes as often as possible, release them in PhysicsManager::release().
	btAlignedObjectArray<btCollisionShape*> collisionShapes;


private:
	void runTests_init();
	void runTests_start();

public:
	void handler( const EventModule::Event& evt );
	void handler2( const EventModule::Event& evt );

};