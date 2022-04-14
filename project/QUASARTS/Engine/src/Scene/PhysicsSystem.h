#pragma once

#include "QuasartsEngine.h"
#include "Core/IManager.h"
#include "btBulletCollisionCommon.h"
#include "glm/vec3.hpp"

namespace Engine {

#define Q_COLLISION_EPSILON			1e-3	// If using unit length = 1 metre, epsilon = 1e-3 is equivalent to 1 millimetre of collision tolerance.
#define Q_RAYCAST_RAY_MIN_LENGTH	1e6
#define Q_MAX_COLLISION_OBJS		1024
#define Q_DEFAULT_SPHERE_RADIUS		1.f

	class QS_API PhysicsSystem : public IManager
	{
		// singleton
	private:
		static PhysicsSystem* instance;
		PhysicsSystem() : collisionConfiguration(nullptr), collisionWorld(nullptr), dispatcher(nullptr), overlappingPairCache(nullptr), numAssignedObjects(0) { };
	public:
		static PhysicsSystem* Instance();
		~PhysicsSystem() {};

	public:
		void init() override;
		int start() override;
		void update() override;
		int stop() override;
		void release() override;


		enum CollisionObjectUsage
		{
			Unassigned,
			Sphere
		};

		// Usage functions //
	public:		
		/// <summary>
		/// Assign a collision object to a collision component. Gets the ID of an unassigned collision object, sets its transform to the given world position, and sets its collision shape to a sphere shape with the given radius.
		/// </summary>
		/// <param name="worldPosition">Position in world coordinates of the assigned collision object.</param>
		/// <param name="radius">Radius of the assigned collision object's sphere shape.</param>
		/// <returns>ID of the assigned collision object, or -1 if an object could not be assigned.</returns>
		int assign_collision_sphere(const glm::vec3 worldPosition, const float radius);

		/// <summary>
		/// Unassign a collision object from a collision component.
		/// </summary>
		/// <param name="id">ID of the collision object to be unassigned, stored in the associated collision component.</param>
		void unassign_collision_object(const int id);

		/// <summary>
		/// Casts a ray from the given origin along the given direction vector. Raycasting stops at the first detected hit. Ray has finite length: ray.length = direction.length if direction.length >= Q_RAYCAST_RAY_MIN_LENGTH, otherwise ray.length = Q_RAYCAST_RAY_MIN_LENGTH.
		/// </summary>
		/// <param name="origin">Ray world origin.</param>
		/// <param name="direction">Ray world direction.</param>
		/// <param name="hitLocation">Address of vector to store the world location of a detected hit.</param>
		/// <returns>true is there is a hit, false otherwise.</returns>
		bool raycast(const glm::vec3 origin, const glm::vec3 direction, glm::vec3* hitLocation);


		// Collision world variables //
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btCollisionWorld* collisionWorld;
		// Store collision shapes:
		// Re-use collision shapes as often as possible, release them in PhysicsManager::release().
		btAlignedObjectArray<btSphereShape*> collisionSpheres;

		// Array describing the usage of each pre-constructed collision object in the collision world's collision object array.
		CollisionObjectUsage collisionObjectArrayUsage[Q_MAX_COLLISION_OBJS];
		int numAssignedObjects;


		// Util //
	private:
		int get_unassigned_collision_object_index();

		/// <summary>
		/// Return a pointer to a btSphereShape with the given radius. Adds a new btSphereShape to the collisionSpheres array if none with the given radius is found.
		/// </summary>
		/// <param name="radius">Radius of the required collision sphere.</param>
		/// <returns>Pointer to a btSphereShape in the collisionSpheres array.</returns>
		btSphereShape* get_sphere_shape(float radius);

		/// <summary>
		/// Casts a ray from the given origin along the given direction vector. Raycasting stops at the first detected hit. Ray has finite length: ray.length = direction.length if direction.length >= Q_RAYCAST_RAY_MIN_LENGTH, otherwise ray.length = Q_RAYCAST_RAY_MIN_LENGTH.
		/// </summary>
		/// <param name="origin">Ray world origin.</param>
		/// <param name="direction">Ray world direction.</param>
		/// <param name="hitLocation">Address of vector to store the world location of a detected hit.</param>
		/// <returns>true is there is a hit, false otherwise.</returns>
		bool raycast(const btVector3 origin, const btVector3 direction, btVector3* hitLocation);

		// Math lib conversions
		static btVector3 glm_to_bt_vec3(const glm::vec3 glm_vec) { return btVector3(glm_vec.x, glm_vec.y, glm_vec.z); }
		static glm::vec3 bt_to_glm_vec3(const btVector3 bt_vec) { return glm::vec3(bt_vec.x(), bt_vec.y(), bt_vec.z()); }

		// Debug
		static std::string object_to_string(const btCollisionObject* obj, const bool angles_to_deg = true);
		static std::string transform_to_string(const btTransform* trf, const bool angles_to_deg = true);
		static std::string shape_to_string(const btCollisionShape* shape);
		static std::string vector_to_string(const btVector3 vec);

		// Tests //
	private:
		void runTests_init();
		void runTests_start();

		std::vector<int> testObjIds;


		// Events //
	public:
		void EV_CALLBACK_SIGNATURE(DebugEvent);

	};

}