#ifndef H_PHYISCS
#define H_PHYSICS
#pragma once

#include "btBulletDynamicsCommon.h"
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>

class Physics{
public:
	Physics();
	~Physics();

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;


protected:
	static Physics * physics;
};

#endif