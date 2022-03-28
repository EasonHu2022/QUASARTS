#pragma once

#include "Core/IManager.h"
#include "btBulletCollisionCommon.h"
#include "Event/EventModule.h"
#include "Core/Core.h"

namespace Engine {

#define Q_COLLISION_EPSILON 1e-3

	class QS_API PhysicsSystem : public IManager
	{
		// singleton
	private:
		static PhysicsSystem* instance;
		PhysicsSystem() {};
	public:
		static PhysicsSystem* Instance();
		~PhysicsSystem() {};

	public:
		void init();
		int start();
		void update();
		int stop();
		void release();


		// Usage functions //
	public:
		void create_collision_sphere(float radius);


		// Collision world //
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btCollisionWorld* collisionWorld;
		// Store collision shapes.
		// Re-use collision shapes as often as possible, release them in PhysicsManager::release().
		btAlignedObjectArray<btSphereShape*> collisionSpheres;


		// Util //
	private:
		static std::string object_to_string(const btCollisionObject* obj, const bool angles_to_deg = true);
		static std::string transform_to_string(const btTransform* trf, const bool angles_to_deg = true);
		static std::string shape_to_string(const btCollisionShape* shape);

		// Tests //
	private:
		void runTests_init();
		void runTests_start();


		// Events //
	public:
		void EV_CALLBACK_SIGNATURE(DebugEvent);

	};

}