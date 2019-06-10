#ifndef H_COMPONENT_GOAL
#define H_COMPONENT_GOAL
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"


class ComponentGoal : public Component {
public:
	bool isActive = true;

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
	bool scored = false;
	float scoreTime = 1.0f;
	float curTime = 0.0f;
	int scoreSound = 2;

	void update(double deltatime) {
		glm::vec3 ballPos = ball->getPosition(false);
		if (!scored) {
			if (checkExtents(tExtents.x, bExtents.x, ballPos.x)) {
				if (checkExtents(tExtents.y, bExtents.y, ballPos.y)) {
					if (checkExtents(-tExtents.z, -bExtents.z, -ballPos.z)) {
						std::cout << "goal my dude\n";
						ProjectManager::project->getSoundEffect(HIT_SOUND)->Play(0, 0, 0, scoreSound);
						scored = true;
					}
				}
			}
		}
		else {
			golazo(deltatime);
		}
	}

	void golazo(double deltatime) {
		curTime += (float)deltatime;
		if (curTime > scoreTime) {
			btTransform bt;
			bt.setIdentity();
			bt.setOrigin(btVector3(0, 50, 0));
			ball->rigidBody->setWorldTransform(bt);
			ball->rigidBody->getMotionState()->setWorldTransform(bt);
			glm::mat4 reset = glm::mat4(1);
			reset[3] = glm::vec4(0, 0, 0, 1);
			ball->setToWorld(reset);

			ball->rigidBody->setLinearVelocity(btVector3(0, 0, 0));

			curTime = 0;
			scored = false;
		}
	}

	bool checkExtents(float max, float min, float ball) {
		return (ball > min) && (ball < max);
	}

};

#endif
