#include "IManager.h"


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
	

};