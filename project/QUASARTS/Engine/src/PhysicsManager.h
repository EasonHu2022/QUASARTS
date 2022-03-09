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
	virtual void init();
	virtual int start();
	virtual void update();
	virtual int stop();
	virtual void release();


private:
	btDefaultCollisionConfiguration*		collisionConfiguration;
	btCollisionDispatcher*					dispatcher;
	btBroadphaseInterface*					overlappingPairCache;
	btCollisionWorld*						collisionWorld;
	// Store collision shapes.
	// Re-use collision shapes as often as possible, release them in PhysicsManager::release().
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

};