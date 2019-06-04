#ifndef H_PHYSICS
#define H_PHYSICS
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "btBulletDynamicsCommon.h"

#include "Definitions.h"

class Physics{
public:
	static Physics * physics;

	Physics();
	~Physics();

	void update(double deltaTime);
	void draw(glm::mat4 headPose, glm::mat4 projection);

	static btRigidBody* addPlaneCollider(float size, glm::vec3 position, glm::vec3 axis = AXIS_Y_POSITIVE);
	static btRigidBody* addSphereCollider(float radius, glm::vec3 position);
	static btRigidBody* addBoxCollider(glm::vec3 size, glm::vec3 position);

	static btRigidBody* getRigidbody(int index);
	static int getCollisionObjectCount() { return physics->dynamicsWorld->getNumCollisionObjects(); }

private:
	btRigidBody * groundCollider = 0;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

};

namespace bullet {
	inline glm::vec3 ToGlm(const btVector3& v) { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

	inline btVector3 fromGlm(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }

	inline glm::quat ToGlm(const btQuaternion& q) { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

	inline btQuaternion fromGlm(const glm::quat& q) { return btQuaternion(q.x, q.y, q.z, q.w); }

	inline btMatrix3x3 fromGlm(const glm::mat3& m) { return btMatrix3x3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]); }

	// btTransform does not contain a full 4x4 matrix, so this transform is lossy.
	// Affine transformations are OK but perspective transformations are not.
	inline btTransform fromGlm(const glm::mat4& m) {
		glm::mat3 m3(m);
		return btTransform(fromGlm(m3), fromGlm(glm::vec3(m[3][0], m[3][1], m[3][2])));
	}

	inline glm::mat4 ToGlm(const btTransform& t) {
		glm::mat4 m = glm::mat4(1);
		const btMatrix3x3& basis = t.getBasis();
		// rotation
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				m[c][r] = basis[r][c];
			}
		}
		// traslation
		btVector3 origin = t.getOrigin();
		m[3][0] = origin.getX();
		m[3][1] = origin.getY();
		m[3][2] = origin.getZ();
		// unit scale
		m[0][3] = 0.0f;
		m[1][3] = 0.0f;
		m[2][3] = 0.0f;
		m[3][3] = 1.0f;
		return m;
	}
};

#endif