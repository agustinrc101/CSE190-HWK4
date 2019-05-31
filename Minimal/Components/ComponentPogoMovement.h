#ifndef H_COMPONENT_POGO_MOVEMENT
#define H_COMPONENT_POGO_MOVEMENT
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"

class ComponentPogoMovement : public Component {
public:
	ComponentPogoMovement(Transform * h, Transform * left, Transform * right, Transform * pogo) : head(h), handL(left), handR(right), collider(pogo)  {
		storedColorL = handL->material->color;
		storedColorR = handR->material->color;
	}
	~ComponentPogoMovement() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		switch (state) {
		case IDLE:
			handleIdleState();
			break;
		case ARM_UP_AWAY:
			handleArmUpAwayState();
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
		ARM_UP_AWAY = 1,
		ARM_DOWN = 2,
	};

	Transform * head;
	Transform * handL;
	Transform * handR;
	Transform * collider;

	glm::vec3 storedColorL;
	glm::vec3 storedColorR;

	glm::vec3 storedHandPosition;
	glm::vec3 prevHandRPosition;

	float maxDistance = 0.55f;
	float minDistance = 0.3f;

	MovementState state = IDLE;

	void handleIdleState() {
		if (handR->getPosition().y >= head->getPosition().y) {

			handR->material->color = glm::vec3(COLOR_WHITE);
			state = ARM_UP_AWAY;
		}
		else {
			handR->material->color = storedColorR;
		}
	}

	void handleArmUpAwayState() {
		//if (handR->getPosition().y < (head->getPosition().y - 0.15f)) {
		//print(collider->getPosition(false));
		if(collider->getPosition(false).y <= -2){
			handR->material->color = glm::vec3(COLOR_BLACK);
			storedHandPosition = handR->getPosition(false);
			prevHandRPosition = handR->getPosition(false);
			state = ARM_DOWN;

		}
		else {
			handR->material->color = glm::vec3(COLOR_WHITE);
		}
	}

	void handleArmDownState() {
		glm::vec3 dir = prevHandRPosition - handR->getPosition(false);
		dir.y = 0;
		dir.z = dir.z;

		transform->translate(dir);

		//Lock
		prevHandRPosition = handR->getPosition(false);
		glm::vec3 yMove = prevHandRPosition - storedHandPosition;	yMove.x = yMove.z = 0;
		if (yMove.y < 0)	handR->setPosition(storedHandPosition, false);
		else				handR->setPosition(storedHandPosition + yMove, false);

		if (testMinHorizontalDistance(handR->getPosition(false), head->getPosition(false))) {
			handR->material->color = storedColorR;
			state = IDLE;
		}
		else if (handR->getPosition().y >= head->getPosition().y) {

			handR->material->color = glm::vec3(COLOR_WHITE);
			state = ARM_UP_AWAY;
		}
		//else if (testMaxHorizontalDistance(handR->getPosition(false), head->getPosition(false))) {
		//	handR->material->color = storedColorR;
		//	state = IDLE;
		//}
		else {
			handR->material->color = glm::vec3(COLOR_BLACK);
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
		std::cout << distance(a, b) << std::endl;
		return distance(a, b) < minDistance;
	}
};

#endif