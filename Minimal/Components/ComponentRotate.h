#ifndef H_COMPONENT_ROTATE
#define H_COMPONENT_ROTATE
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"
#include "Definitions.h"

class ComponentRotate : public Component {
public:
	ComponentRotate(glm::vec3 axisOfRotation = AXIS_Y_POSITIVE, float rotationSpeed = 1.0f, float rotationAngle = 45.0f)
		: axis(axisOfRotation), speed(rotationSpeed), angle(rotationAngle) { }
	~ComponentRotate() {}

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
		
	}
	
};

#endif