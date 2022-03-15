#include "IManager.h"
#include "btBulletCollisionCommon.h"


class PhysicsManager : public IManager
{
// singleton
private:
	static PhysicsManager* instance;
public:
	static PhysicsManager* Instance();
private:
	PhysicsManager() {};
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

};