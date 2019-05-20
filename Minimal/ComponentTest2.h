#ifndef H_COMPONENT_TEST2
#define H_COMPONENT_TEST2
#pragma once

#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "Definitions.h"

class ComponentTest2 : public Component {
public:
	ComponentTest2(glm::vec3 axisOfRotation = AXIS_Y_POSITIVE, float rotationSpeed = 1.0f, float rotationAngle = 45.0f)
		: axis(axisOfRotation), speed(rotationSpeed), angle(rotationAngle) { }
	~ComponentTest2() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override { 
		transform->rotate(angle * (float)deltaTime * speed, axis);
	}

protected:
	Transform * transform;
	glm::vec3 axis;
	float angle;
	float speed;

	void Start() override {
		std::cout << "ComponentTest2 Start" << std::endl;
	}
	
};

#endif