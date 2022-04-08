#pragma once

#include "Core/IManager.h"
#include "btBulletCollisionCommon.h"
#include "Event/EventModule.h"
#include "Core/Core.h"
#include "glm/vec3.hpp"

namespace Engine {

#define Q_COLLISION_EPSILON			1e-3	// If using unit length = 1 metre, epsilon = 1e-3 is equivalent to 1 millimetre of collision tolerance.
#define Q_RAYCAST_RAY_MIN_LENGTH	1e6

	class QS_API PhysicsSystem : public IManager
	{
		// singleton
	private:
		static PhysicsSystem* instance;
		PhysicsSystem() : collisionConfiguration(nullptr), collisionWorld(nullptr), dispatcher(nullptr), overlappingPairCache(nullptr) {};
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
		void create_collision_sphere(const float radius);
		bool raycast(const glm::vec3 origin, const glm::vec3 direction, glm::vec3* hitLocation);


		// Collision world //
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btCollisionWorld* collisionWorld;
		// Store collision shapes:
		// Re-use collision shapes as often as possible, release them in PhysicsManager::release().
		btAlignedObjectArray<btSphereShape*> collisionSpheres;


		// Util //
	private:
		bool raycast(const btVector3 origin, const btVector3 direction, btVector3* hitLocation);

		static btVector3 glm_to_bt_vec3(const glm::vec3 glm_vec) { return btVector3(glm_vec.x, glm_vec.y, glm_vec.z); }
		static glm::vec3 bt_to_glm_vec3(const btVector3 bt_vec) { return glm::vec3(bt_vec.x(), bt_vec.y(), bt_vec.z()); }

		static std::string object_to_string(const btCollisionObject* obj, const bool angles_to_deg = true);
		static std::string transform_to_string(const btTransform* trf, const bool angles_to_deg = true);
		static std::string shape_to_string(const btCollisionShape* shape);
		static std::string vector_to_string(const btVector3 vec);

		// Tests //
	private:
		void runTests_init();
		void runTests_start();


		// Events //
	public:
		void EV_CALLBACK_SIGNATURE(DebugEvent);

	};

}