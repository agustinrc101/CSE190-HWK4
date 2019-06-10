#ifndef H_COMPONENT_BALL_PROPERTIES
#define H_COMPONENT_BALL_PROPERTIES
#pragma once

#include <iostream>
#include "../Transform.h"
#include "../Definitions.h"
#include "../Input.h"
#include "../Networking/Client.h"
#include "Component.h"
#include "Physics.h"
#include "btBulletDynamicsCommon.h"
#include <ctime>

class ComponentBallProperties : public Component {
public:
	bool isActive = true;

	ComponentBallProperties(ComponentRigidBodyStick * leftStick, ComponentRigidBodyStick * rightStick) : stickL(leftStick), stickR(rightStick) {}
	~ComponentBallProperties() {}

	//////Sound Properties/////
	float delayTime = 0.3f;	
	float delay = 0.0f;
	bool hasHit = false;
	float delay2 = 0.0f;
	bool hasHit2 = false;

	int hitSound = 1;
	//////Sound Properties/////

	void Init(Transform * p) {
		transform = p;
	}
	int up = 0;
	void Update(double deltaTime) override {
		if (hasHit) {
			delay += (float)deltaTime;
			if (delay > delayTime) {
				hasHit = false;
				delay = 0;
			}
		}
		if (hasHit2) {
			delay2 += (float)deltaTime;
			if (delay2 > delayTime) {
				hasHit2 = false;
				delay2 = 0;
			}
		}
		int numManifolds = Physics::physics->dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++) {
			btPersistentManifold* contactManifold = Physics::physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			if ((obA->getCollisionShape()->getUserIndex() == LAYER_BALL) || (obB->getCollisionShape()->getUserIndex() == LAYER_BALL) ) {
				if (((obA->getCollisionShape()->getUserIndex() == LAYER_STICK_LEFT) || (obB->getCollisionShape()->getUserIndex() == LAYER_STICK_LEFT)) && contactManifold->getNumContacts() >= 1) {
					if (!hasCollidedL) {
						hasCollidedL = true;
						if (!hasHit) {
							glm::vec4 soundPos = bullet::ToGlm(rigidbody->getWorldTransform())[3];
							ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(soundPos.x, soundPos.y, soundPos.z, hitSound);
							Client::client->sendAudioPacket(hitSound, soundPos);
							hasHit = true;
						}
					}
					
					isCollidingL = true;
				}
				else {
					isCollidingL = false;
				}
				if(((obA->getCollisionShape()->getUserIndex() == LAYER_STICK_RIGHT) || (obB->getCollisionShape()->getUserIndex() == LAYER_STICK_RIGHT)) && contactManifold->getNumContacts() >= 1) {
					if (!hasCollidedR) {
						hasCollidedR = true;
						if (!hasHit2) {
							glm::vec4 soundPos = bullet::ToGlm(rigidbody->getWorldTransform())[3];
							ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(soundPos.x, soundPos.y, soundPos.z, hitSound);
							Client::client->sendAudioPacket(hitSound, soundPos);
							hasHit2 = true;
						}
					}
					isCollidingR = true;
				}
				else {
					isCollidingR = false;
				}
				if (((obA->getCollisionShape()->getUserIndex() == LAYER_OTHER_STICK_LEFT) || (obB->getCollisionShape()->getUserIndex() == LAYER_OTHER_STICK_LEFT)) && contactManifold->getNumContacts() >= 1) {
					if (!hasCollidedL) {
						hasCollidedL = true;
						if (!hasHit) {
							glm::vec4 soundPos = bullet::ToGlm(rigidbody->getWorldTransform())[3];
							ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(soundPos.x, soundPos.y, soundPos.z, hitSound);
							Client::client->sendAudioPacket(hitSound, soundPos);
							hasHit = true;
						}
					}

					isCollidingL = true;
				}
				else {
					isCollidingL = false;
				}
				if (((obA->getCollisionShape()->getUserIndex() == LAYER_OTHER_STICK_RIGHT) || (obB->getCollisionShape()->getUserIndex() == LAYER_OTHER_STICK_RIGHT)) && contactManifold->getNumContacts() >= 1) {
					if (!hasCollidedR) {
						hasCollidedR = true;
						if (!hasHit2) {
							glm::vec4 soundPos = bullet::ToGlm(rigidbody->getWorldTransform())[3];
							ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(soundPos.x, soundPos.y, soundPos.z, hitSound);
							Client::client->sendAudioPacket(hitSound, soundPos);
							hasHit2 = true;
						}
					}
					isCollidingR = true;
				}
				else {
					isCollidingR = false;
				}
			}
		}

		if (!isCollidingL && hasCollidedL) {
			hasCollidedL = false;
		}
		else if (!isCollidingR && hasCollidedR) {
			hasCollidedR = false;
		}

		if (Input::getButtonStickL() || Input::getButtonStickR()) {
			if (!button_press) {
				button_press = true;
				btTransform bt;
				bt.setIdentity();

				rigidbody->setWorldTransform(bt);
				rigidbody->getMotionState()->setWorldTransform(bt);
				transform->setToWorld(glm::mat4(1));
				rigidbody->setLinearVelocity(btVector3(0, 0, 0));
			}
		}
		else
			button_press = false;

	}

	void LateInit() override {
		Start();
	}

protected:
	Transform * transform;
	btRigidBody * rigidbody;

	void Start() override {
		rigidbody = transform->rigidBody;
		rigidbody->setRestitution(0.5f);			//Bouncyness
		rigidbody->setFriction(0.5f);				//Friction
		//rigidbody->setRollingFriction(0.075f);		//Rolling Friction

		rigidbody->getCollisionShape()->setUserIndex(LAYER_BALL);
	}

private:
	ComponentRigidBodyStick * stickL;
	ComponentRigidBodyStick * stickR;
	bool button_press = false;
	float str = 0.01f;

	bool hasCollidedL = false;
	bool isCollidingL = false;
	bool hasCollidedR = false;
	bool isCollidingR = false;
};

#endif