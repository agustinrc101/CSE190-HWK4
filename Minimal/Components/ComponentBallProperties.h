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
		
			
		
	
		if (Input::getHandTriggerL()) {
			if (!button_press) {
				button_press = true;
				rigidbody->applyForce(bullet::fromGlm(glm::vec3(0, 0.1f, -0.1f)), bullet::fromGlm(transform->getPosition(false)));
			}
		}
		else
			button_press = false;

		int numManifolds = Physics::physics->dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++) {
			btPersistentManifold* contactManifold = Physics::physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();
			
			if ((obA->getCollisionShape()->getUserIndex() == LAYER_BALL && obB->getCollisionShape()->getUserIndex() == LAYER_STICK_LEFT)) {
				//btVector3 dir = (bullet::fromGlm(stickL->getVelocity())).normalize() * str;
				//rigidbody->applyCentralImpulse(dir);
			}
			else if (obA->getCollisionShape()->getUserIndex() == LAYER_STICK_LEFT && obB->getCollisionShape()->getUserIndex() == LAYER_BALL) {
				//btVector3 dir = bullet::fromGlm(stickL->getVelocity()).normalize() * str;
				//rigidbody->applyCentralImpulse(dir);
			}
			/*else if ((obA->getCollisionShape()->getUserIndex() == LAYER_BALL && obB->getCollisionShape()->getUserIndex() == LAYER_STICK_RIGHT)) {
				btVector3 dir = bullet::fromGlm(stickR->getVelocity()) * str;
				rigidbody->applyImpulse(dir, btVector3(0, 0, 0));
			}
			else if (obA->getCollisionShape()->getUserIndex() == LAYER_STICK_RIGHT && obB->getCollisionShape()->getUserIndex() == LAYER_BALL) {
				btVector3 dir = bullet::fromGlm(stickR->getVelocity()) * str;
				rigidbody->applyImpulse(dir, btVector3(0,0,0));
			}*/
		}

		str += Input::getStickL().x;
		if (Input::getStickL().x != 0) std::cout << str << std::endl;
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
		rigidbody->setFriction(1.0f);			//Friction
		rigidbody->setRollingFriction(1.0f);	//Rolling Friction

		rigidbody->getCollisionShape()->setUserIndex(LAYER_BALL);
	}

private:
	ComponentRigidBodyStick * stickL;
	ComponentRigidBodyStick * stickR;
	bool button_press = false;
	float str = 0.01f;
};

#endif