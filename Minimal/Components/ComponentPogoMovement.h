#ifndef H_COMPONENT_POGO_MOVEMENT
#define H_COMPONENT_POGO_MOVEMENT
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"
#include "ComponentRigidBodyStick.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/quaternion.hpp"

class ComponentPogoMovement : public Component {
public:
	bool isActive = true;
	
	ComponentPogoMovement(Transform * theHead, Transform * theHand, Transform * pogoCollider, ComponentRigidBodyStick * theStickCollider, ComponentRigidBodyStick * theOtherStickCollider, Transform * handModel)
		: head(theHead), hand(theHand), collider(pogoCollider), stickCollider(theStickCollider), otherStickCollider(theOtherStickCollider), handTransform (handModel)  {
		storedColor = hand->material->color;
	}
	~ComponentPogoMovement() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		//std::cout << state << std::endl;
		switch (state) {
		case IDLE:
			handleIdleState();
			break;
		case ARM_UP:
			handleArmUpState();
			break;
		case ARM_DOWN:
			handleArmDownState();
			break;
		default:
			state = IDLE;
			break;
		}
	}

protected:
	Transform * transform;
	Transform * handTransform;

	void Start() override {
		
	}

private:

	enum MovementState {
		IDLE = 0,
		ARM_UP = 1,
		ARM_DOWN = 2,
	};
	enum HandState {
		BOTH_IDLE = 0,
		LEFT = 1,
		RIGHT = 2,
	};

	Transform * head;
	Transform * hand;
	Transform * collider;
	ComponentRigidBodyStick * stickCollider;
	ComponentRigidBodyStick * otherStickCollider;

	glm::vec3 storedColor;

	glm::vec3 storedHandPosition;
	glm::vec3 prevHandPosition;

	float maxDistance = 0.55f;
	float minDistance = 0.3f;

	MovementState state = IDLE;
	HandState handState = BOTH_IDLE;

	int hitSound = 3;

	void handleIdleState() {
		if (hand->getPosition().y >= (head->getPosition().y / 1.75f)) {
			handState = RIGHT;
			hand->material->color = glm::vec3(COLOR_WHITE);
			state = ARM_UP;
		}
		else {
			hand->material->color = storedColor;
		}

	}

	void handleArmUpState() {
		if (hand->getPosition().y < (head->getPosition().y - 0.15f) && collider->getPosition(false).y <= 0.0f) {
		/*if(stickCollider->touchingGround){*/
			hand->material->color = glm::vec3(COLOR_BLACK);
			storedHandPosition = handTransform->getPosition(false);
			prevHandPosition = handTransform->getPosition(false);
			state = ARM_DOWN;
			ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(collider->getPosition(false).x, collider->getPosition(false).y, collider->getPosition(false).z, hitSound);
		}
		else {
			hand->material->color = glm::vec3(COLOR_WHITE);
		}
	}

	void handleArmDownState() {
		if (collider->getPosition(false).y > -1.3f) {
			hand->material->color = storedColor;
			stickCollider->isWalking = false;
			state = IDLE;
			//transform->translate(dir);

		}
		else {
			stickCollider->isWalking = true;
			glm::vec3 dir = prevHandPosition - handTransform->getPosition(false);

			dir.y = 0;
			dir.z = dir.z;

			glm::mat4 m = glm::mat4(1);
			m[3] = glm::vec4(dir, 1);

			glm::mat4 p = transform->getCompleteToWorld();
			p[3] = glm::vec4(0, 0, 0, 1);

			m = p * m;

			dir = m[3];
			if(otherStickCollider->isWalking)
				transform->translate(dir);
			else {
				if (Input::getHandTriggerL() > 0.5f && Input::getHandTriggerR() > 0.5f)
					transform->translate(1.85f * dir);	//I AM SPEED
				else
					transform->translate(dir);
			}
			

			//Lock
			prevHandPosition = hand->getPosition(false);
			glm::vec3 yMove = prevHandPosition - storedHandPosition;	yMove.x = yMove.z = 0;
			if (yMove.y < 0)	hand->setPosition(storedHandPosition, false);
			else				hand->setPosition(storedHandPosition + yMove, false);

			if (testMinHorizontalDistance(hand->getPosition(false), head->getPosition(false))) {
				hand->material->color = storedColor;
				//state = IDLE;
				//state = ARM_UP;
			}
			else if (hand->getPosition().y >= head->getPosition().y * .7f) {
				//else if (!stickCollider->touchingGround) {

				hand->material->color = glm::vec3(COLOR_WHITE);
				stickCollider->isWalking = false;
				state = ARM_UP;
			}

			else {
				hand->material->color = glm::vec3(COLOR_BLACK);
			}
		}
		
	}



	bool testMaxHorizontalDistance(glm::vec3 v1, glm::vec3 v2) {
		glm::vec3 a = glm::vec3(v1.x, 0, v1.z);
		glm::vec3 b = glm::vec3(v2.x, 0, v2.z);

		return distance(a, b) > maxDistance;
	}

	bool testMinHorizontalDistance(glm::vec3 v1, glm::vec3 v2) {
		glm::vec3 a = v1;//glm::vec3(v1.x, 0, v1.z);
		glm::vec3 b = v2;//glm::vec3(v2.x, 0, v2.z);
		
		return distance(a, b) < minDistance;
	}
};

#endif