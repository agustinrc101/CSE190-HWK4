#ifndef H_COMPONENT_MOVE_TEST
#define H_COMPONENT_MOVE_TEST
#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "Component.h"
#include "Transform.h"

class ComponentMoveTest : public Component {
public:
	bool isActive = true;

	ComponentMoveTest(glm::vec3 direction, float moveTime = 1.0f, float moveSpeed = 1.0f)
		: dir(direction), time(moveTime), speed(moveSpeed) {}
	~ComponentMoveTest() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		curTime += (float)deltaTime;
		if (curTime > time) {
			curTime = 0;
			curDir *= -1.0f;
		}

		glm::vec3 translation = dir * (float)deltaTime * speed * curDir;
		transform->translate(translation, false);
	}

protected:
	Transform * transform;

	void Start() override {
		
	}

private:
	glm::vec3 dir;
	float curDir = 1;
	float time;
	float curTime = 0;
	float speed;
	
};


#endif
