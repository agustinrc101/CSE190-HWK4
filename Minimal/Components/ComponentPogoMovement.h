#ifndef H_COMPONENT_POGO_MOVEMENT
#define H_COMPONENT_POGO_MOVEMENT
#pragma once

#include <iostream>
//#include "../Transform.h"
//#include "Component.h"
#include "ComponentRigidBodyStick.h"

class ComponentPogoMovement : public Component {
public:
	ComponentPogoMovement(Transform * theHead, Transform * theHand, Transform * pogoCollider, ComponentRigidBodyStick * theStickCollider) : head(theHead), hand(theHand), collider(pogoCollider), stickCollider(theStickCollider)  {
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

	glm::vec3 storedColor;

	glm::vec3 storedHandPosition;
	glm::vec3 prevHandPosition;

	float maxDistance = 0.55f;
	float minDistance = 0.3f;

	MovementState state = IDLE;
	HandState handState = BOTH_IDLE;

	int hitSound = 3;

	void handleIdleState() {
		if (hand->getPosition().y >= (head->getPosition().y / 1.5f)) {
			handState = RIGHT;
			hand->material->color = glm::vec3(COLOR_WHITE);
			state = ARM_UP;
		}
		else {
			hand->material->color = storedColor;
		}

	}

	void handleArmUpState() {
		if (hand->getPosition().y < (head->getPosition().y - 0.15f) && collider->getPosition(false).y <= -1.0f) {
		/*if(stickCollider->touchingGround){*/
			hand->material->color = glm::vec3(COLOR_BLACK);
			storedHandPosition = hand->getPosition(false);
			prevHandPosition = hand->getPosition(false);
			state = ARM_DOWN;
			ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(collider->getPosition(false).x, collider->getPosition(false).y, collider->getPosition(false).z, hitSound);
		}
		else {
			hand->material->color = glm::vec3(COLOR_WHITE);
		}
	}

	void handleArmDownState() {
		if (collider->getPosition(false).y > -1.2f) {
			hand->material->color = storedColor;
			state = IDLE;
			
		}


		glm::vec3 dir = prevHandPosition - hand->getPosition(false);
		dir.y = 0;
		dir.z = dir.z;
	
		transform->translate(dir);

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
			state = ARM_UP;
		}
		
		else {
			hand->material->color = glm::vec3(COLOR_BLACK);
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