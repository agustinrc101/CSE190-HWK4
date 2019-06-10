#include "Physics.h"
#include "Input.h"

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

	dynamicsWorld->setGravity(btVector3(0, -7, 0));

	//Debug Drawing
	debugDrawer = new BulletDebugDrawer_OpenGL();
	dynamicsWorld->setDebugDrawer(debugDrawer);
}

bool debug = true;
void Physics::draw(glm::mat4 headPose, glm::mat4 projection) {
	if (debug && (Input::getIndexTriggerL() || Input::getIndexTriggerR())) {		//TODO remove this
		debugDrawer->SetMatrices(headPose, projection);
		dynamicsWorld->debugDrawWorld();
	}
}

void Physics::update(double deltaTime) {
	dynamicsWorld->stepSimulation((btScalar)deltaTime, 5);
	dynamicsWorld->updateAabbs();
}

void Physics::newLColPos(glm::vec3 position, glm::quat orientation, glm::vec3 velocity) {
	btTransform newPos;
	btQuaternion  ori;
	//ori = bullet::fromGlm(orientation);
	ori = btQuaternion(0, 0, 0, 1);
	newPos.setOrigin(btVector3(position.x, position.y, position.z));
	newPos.setRotation(bullet::fromGlm(orientation));
	if (lHandCol) {
		//lHandCol->clearForces();
		//btVector3 zeroVector(0, 0, 0);
		//lHandCol->setLinearVelocity(zeroVector);
		lHandCol->setWorldTransform(newPos);
		lHandCol->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
		
		
	}
}

void Physics::newRColPos(glm::vec3 position, glm::quat orientation, glm::vec3 velocity) {
	btTransform newPos;
	btQuaternion  ori;
	//ori = bullet::fromGlm(orientation);
	ori = btQuaternion(0, 0, 0, 1);
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

void Physics::newOtherLColPos(glm::vec3 position, glm::quat orientation, glm::vec3 velocity) {
	btTransform newPos;
	btQuaternion  ori;
	ori = bullet::fromGlm(orientation);
	newPos.setOrigin(btVector3(position.x, position.y, position.z));
	newPos.setRotation(ori);
	if (otherLHandCol) {
		otherLHandCol->setWorldTransform(newPos);
		otherLHandCol->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
	}
}

void Physics::newOtherRColPos(glm::vec3 position, glm::quat orientation, glm::vec3 velocity) {
	btTransform newPos;
	btQuaternion  ori;
	ori = bullet::fromGlm(orientation);
	newPos.setOrigin(btVector3(position.x, position.y, position.z));
	newPos.setRotation(ori);
	if (otherRHandCol) {
		otherRHandCol->setWorldTransform(newPos);
		otherRHandCol->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
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

	shape->setUserIndex(LAYER_GROUND_MOVEMENT);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	
	body->setRestitution(1.0f);
	body->setFriction(0.6f);
	body->setRollingFriction(0.005f);
	body->setSpinningFriction(0.15f);

	//add to collisionshapes
	physics->collisionShapes.push_back(shape);
	//add the body to the dynamics world
	physics->dynamicsWorld->addRigidBody(body);

	//Return rigidbody
	return body;
}

btRigidBody* Physics::addStickCollider(glm::vec3 size, glm::vec3 position, bool leftHand, bool thisPlayer) {
	//Create shape
	btCollisionShape* shape = new btBoxShape(bullet::fromGlm(size));

	//Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(0.0f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	
	//body->setRestitution(1.0f);
	//body->setFriction(.10f);
  
	if (!leftHand) {
		if (thisPlayer)
			physics->rHandCol = body;
		else
			physics->otherRHandCol = body;
	}
	else {
		if (thisPlayer)
			physics->lHandCol = body;
		else
			physics->otherLHandCol = body;
	}

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

	btScalar mass(0.15f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);



	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setActivationState(4);
	
	body->setRestitution(1.0f);
	//body->setFriction(0.5f);	
	
	//body->setRollingFriction(0.05f);
	

	//add to collisionshapes
	physics->collisionShapes.push_back(shape);
	//add the body to the dynamics world
	physics->dynamicsWorld->addRigidBody(body);

	return body;
}

///Adds object
btRigidBody* Physics::addBoxCollider(glm::vec3 size, glm::vec3 position, float Mass) {
	//create a dynamic rigidbody

	btCollisionShape* shape = new btBoxShape(btVector3(size.x, size.y, size.z));

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