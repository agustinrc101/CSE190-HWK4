#ifndef H_COMPONENT_RIGIDBODYPLANE
#define H_COMPONENT_RIGIDBODYPLANE
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Physics.h"
#include "../Transform.h"
#include "Component.h"
#include "btBulletDynamicsCommon.h"


class ComponentRigidBodyPlane : public Component {
public:
	bool isActive = true;

	ComponentRigidBodyPlane(float size, glm::vec3 Axis=AXIS_Y_NEGATIVE) : colliderSize(size), axis(Axis) {}
	~ComponentRigidBodyPlane() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		update();
	}

protected:
	Transform * transform;
	btRigidBody * rigidbody;

	float colliderSize;
	glm::vec3 axis;

	void Start() override {
		rigidbody = transform->rigidBody = Physics::addPlaneCollider(colliderSize, transform->getPosition(false), axis);
	}

	void update() {
		
	}

};

#endif
