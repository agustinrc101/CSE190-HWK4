#ifndef H_COMPONENT_TEST4
#define H_COMPONENT_TEST4
#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include "Component.h"
#include "Transform.h"

class ComponentTest4 : public Component {
public:
	ComponentTest4(glm::vec3 direction, float moveTime = 1.0f, float moveSpeed = 1.0f) 
		: dir(direction), time(moveTime), speed(moveSpeed) {}
	~ComponentTest4() {}

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
		std::cout << "ComponentTest4 Start" << std::endl;
	}

private:
	glm::vec3 dir;
	float curDir = 1;
	float time;
	float curTime = 0;
	float speed;
	
};


#endif
