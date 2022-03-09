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
	QDEBUG("Test for PhysicsManager init()");

}

int PhysicsManager::start()
{
	QDEBUG("Test for PhysicsManager start()");

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

}