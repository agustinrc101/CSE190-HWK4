#ifndef H_COMPONENT_TEST2
#define H_COMPONENT_TEST2
#pragma once

#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "Definitions.h"

class ComponentTest2 : public Component {
public:
	ComponentTest2(glm::vec3 axisOfRotation = AXIS_Y_POSITIVE) { axis = axisOfRotation; }
	~ComponentTest2() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override { 
		transform->rotate(45.0f * deltaTime, axis);
	}

protected:
	Transform * transform;
	glm::vec3 axis;

	void Start() override {
		std::cout << "ComponentTest2 Start" << std::endl;
	}
	
};

#endif