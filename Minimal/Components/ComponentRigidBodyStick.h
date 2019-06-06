#ifndef H_COMPONENT_RIGIDBODYSTICK
#define H_COMPONENT_RIGIDBODYSTICK
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Physics.h"
#include "../Transform.h"
#include "Component.h"
#include "btBulletDynamicsCommon.h"


class ComponentRigidBodyStick : public Component {
public:
	ComponentRigidBodyStick(float size) : colliderSize(size) {}
	~ComponentRigidBodyStick() {}

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

	void Start() override {
		rigidbody = transform->rigidBody = Physics::addStickCollider(colliderSize, transform->getPosition(false));
	}

	void update() {
		
	}

};

#endif
