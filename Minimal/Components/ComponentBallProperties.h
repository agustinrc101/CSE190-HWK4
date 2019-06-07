#ifndef H_COMPONENT_BALL_PROPERTIES
#define H_COMPONENT_BALL_PROPERTIES
#pragma once

#include <iostream>
#include "../Transform.h"
#include "../Definitions.h"
#include "../Input.h"
#include "Component.h"
#include "Physics.h"
#include "btBulletDynamicsCommon.h"
#include <ctime>

class ComponentBallProperties : public Component {
public:
	ComponentBallProperties(ComponentRigidBodyStick * leftStick, ComponentRigidBodyStick * rightStick) : stickL(leftStick), stickR(rightStick) {}
	~ComponentBallProperties() {}

	void Init(Transform * p) {
		transform = p;
	}
	int up = 0;
	void Update(double deltaTime) override {
		int numManifolds = Physics::physics->dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++) {
			btPersistentManifold* contactManifold = Physics::physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();
			

			if ((obA->getCollisionShape()->getUserIndex() == LAYER_BALL) || (obB->getCollisionShape()->getUserIndex() == LAYER_BALL)) {
				if ((obA->getCollisionShape()->getUserIndex() == LAYER_STICK_LEFT) || (obB->getCollisionShape()->getUserIndex() == LAYER_STICK_LEFT) ||
					(obA->getCollisionShape()->getUserIndex() == LAYER_STICK_RIGHT) || (obB->getCollisionShape()->getUserIndex() == LAYER_STICK_RIGHT)) {
					rigidbody->clearForces();
				}
			}
				
		}
	}

	void LateInit() override {
		Start();
	}

protected:
	Transform * transform;
	btRigidBody * rigidbody;

	void Start() override {
		rigidbody = transform->rigidBody;
		rigidbody->setRestitution(0.5f);		//Bouncyness
		rigidbody->setFriction(0.75f);			//Friction
		rigidbody->setRollingFriction(0.075f);	//Rolling Friction

		rigidbody->getCollisionShape()->setUserIndex(LAYER_BALL);
	}

private:
	ComponentRigidBodyStick * stickL;
	ComponentRigidBodyStick * stickR;
	bool button_press = false;
	float str = 0.01f;
};

#endif