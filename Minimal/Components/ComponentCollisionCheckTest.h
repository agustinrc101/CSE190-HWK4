#ifndef H_COMPONENT_COLLISIONCHECKTEST
#define H_COMPONENT_COLLISIONCHECKTEST
#pragma once

#include <iostream>
#include "../Transform.h"
#include "../Physics.h"
#include "Component.h"
#include "btBulletDynamicsCommon.h"


class ComponentCollisionCheckTest : public Component {
public:
	bool isActive = true;

	ComponentCollisionCheckTest() {}
	~ComponentCollisionCheckTest() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		update();
	}

protected:
	Transform * transform;

	void Start() override {
		rigidbody = transform->rigidBody;
	}

private:
	btRigidBody * rigidbody;

	void update() {
		for (int i = 0; i < Physics::physics->getCollisionObjectCount(); i++) {
			if (rigidbody->checkCollideWith(Physics::physics->getRigidbody(i)))
				std::cout << "==COLLIDED==\n";
		}
	}
};

#endif
