#ifndef H_PHYISCS
#define H_PHYSICS
#pragma once

#include "btBulletDynamicsCommon.h"

class Physics{
public:
	static Physics * physics;

	Physics();
	~Physics();

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif