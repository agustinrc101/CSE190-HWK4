#include "Physics.h"
#include "Input.h"
btRigidBody* rHandCol;
btRigidBody* lHandCol;
Physics::~Physics() {
	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}

Physics::Physics(){
	physics = this;

	//collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -9, 0));

	//Debug Drawing
	debugDrawer = new BulletDebugDrawer_OpenGL();
	dynamicsWorld->setDebugDrawer(debugDrawer);
}

void Physics::draw(glm::mat4 headPose, glm::mat4 projection) {
	if (Input::getIndexTriggerL() || Input::getIndexTriggerR()) {		//TODO remove this
		debugDrawer->SetMatrices(headPose, projection);
		dynamicsWorld->debugDrawWorld();
	}
}

void Physics::update(double deltaTime) {
	dynamicsWorld->stepSimulation((btScalar)deltaTime);
}

void Physics::newRColPos(glm::vec3 position, glm::quat orientation, glm::vec3 velocity) {
	btTransform newPos;
	newPos.setOrigin(btVector3(position.x, position.y, position.z));
	newPos.setRotation(bullet::fromGlm(orientation));
	if (rHandCol) {
		
		//rHandCol->clearForces();
		//btVector3 zeroVector(0, 0, 0);
		//rHandCol->setLinearVelocity(zeroVector);
		
			rHandCol->setWorldTransform(newPos);
			rHandCol->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
		
	}
}

void Physics::newLColPos(glm::vec3 position, glm::quat orientation, glm::vec3 velocity) {
	btTransform newPos;
	newPos.setOrigin(btVector3(position.x, position.y, position.z));
	newPos.setRotation(bullet::fromGlm(orientation));
	if (lHandCol) {
		
		//rHandCol->clearForces();
		//btVector3 zeroVector(0, 0, 0);
		//rHandCol->setLinearVelocity(zeroVector);
		
			lHandCol->setWorldTransform(newPos);
	}
}

///Adds box plane collider object
btRigidBody* Physics::addPlaneCollider(float size, glm::vec3 position, glm::vec3 axis) {
	float sideLength = size / 2.0f;

	//Collider
	btCollisionShape * shape = new btBoxShape(btVector3(btScalar(sideLength), btScalar(sideLength), btScalar(sideLength)));

	//Position
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(position.x + (sideLength * axis.x), position.y + (sideLength * axis.y), position.z + (sideLength * axis.z)));

	//Specify mass (for gravity)
	btScalar mass(0.0f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)	
		shape->calculateLocalInertia(mass, localInertia);

	shape->setUserIndex(LAYER_NULL);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	//body->setRestitution(1.0f);
	//body->setFriction(0.75f);	
	//body->setRollingFriction(0.75f);

	//add to collisionshapes
	physics->collisionShapes.push_back(shape);
	//add the body to the dynamics world
	physics->dynamicsWorld->addRigidBody(body);

	//Return rigidbody
	return body;
}

btRigidBody* Physics::addStickCollider(glm::vec3 size, glm::vec3 position, bool leftHand) {
	//create a dynamic rigidbody

	btCollisionShape* shape = new btCylinderShape(bullet::fromGlm(size));
	//btCollisionShape* shape = new btSphereShape(.1f);
	//Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.0f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	shape->setUserIndex(LAYER_NULL);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	if (!leftHand)
		rHandCol = body;
	else
		lHandCol = body;
	//body->setRestitution(1.0f);
	//body->setFriction(0.75f);
	//make its rigidbody accessible
	shape->setUserPointer((void*)body);
	//add to collisionshapes
	physics->collisionShapes.push_back(shape);
	//add the body to the dynamics world
	physics->dynamicsWorld->addRigidBody(body);




	return body;
}

///Adds sphere collider object
btRigidBody* Physics::addSphereCollider(float radius, glm::vec3 position, float Mass) {
	//create a dynamic rigidbody

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	btCollisionShape* shape = new btSphereShape(btScalar(radius));

	//Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(1.0f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	shape->setUserIndex(LAYER_NULL);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setActivationState(4);
	//body->setFriction(0.75f);
	//add to collisionshapes
	physics->collisionShapes.push_back(shape);
	//add the body to the dynamics world
	physics->dynamicsWorld->addRigidBody(body);

	return body;
}

///Adds object
btRigidBody* Physics::addBoxCollider(glm::vec3 size, glm::vec3 position, float Mass) {
	//create a dynamic rigidbody
	glm::vec3 half = size / 2.0f;

	btCollisionShape* shape = new btBoxShape(btVector3(half.x, half.y, half.z));

	//Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(Mass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	shape->setUserIndex(LAYER_NULL);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	//add to collisionshapes
	physics->collisionShapes.push_back(shape);
	//add the body to the dynamics world
	physics->dynamicsWorld->addRigidBody(body);

	return body;
}

///Get rigidbody from the dynamics world
btRigidBody* Physics::getRigidbody(int index) {
	return (btRigidBody *)physics->dynamicsWorld->getCollisionWorld()->getCollisionObjectArray().at(index)->getUserPointer();
}