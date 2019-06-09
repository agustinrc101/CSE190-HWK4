#ifndef H_COMPONENT_GOAL
#define H_COMPONENT_GOAL
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"


class ComponentGoal : public Component {
public:
	ComponentGoal(Transform * Ball, glm::vec3 TopExtents, glm::vec3 BottomExtents) : ball(Ball), top(TopExtents), bottom(BottomExtents) {}
	~ComponentGoal() {}

	void Init(Transform * p) {
		transform = p;
	}

	void Update(double deltaTime) override {
		update(deltaTime);
	}

	void LateInit() {
		Start();
	}

protected:
	Transform * transform;

	void Start() override {
		bExtents = transform->getPosition(false) + bottom;
		tExtents = transform->getPosition(false) + top;
	}

private:
	Transform * ball;
	glm::vec3 top;
	glm::vec3 bottom;
	glm::vec3 bExtents;
	glm::vec3 tExtents;

	void update(double deltatime) {
		glm::vec3 ballPos = ball->getPosition(false);

		if (checkExtents(tExtents.x, bExtents.x, ballPos.x)) {
			if (checkExtents(tExtents.y, bExtents.y, ballPos.y)) {
				if (checkExtents(-tExtents.z, -bExtents.z, -ballPos.z)) {
					std::cout << "goal my dude\n";
					
					//btTransform bt;
					//bt.setIdentity();

					//ball->rigidBody->setWorldTransform(bt);
					//ball->rigidBody->getMotionState()->setWorldTransform(bt);
					//ball->setToWorld(glm::mat4(1));
					//ball->rigidBody->setLinearVelocity(btVector3(0, 0, 0));
				}
			}
		}
	}

	bool checkExtents(float max, float min, float ball) {
		return (ball > min) && (ball < max);
	}

};

#endif
