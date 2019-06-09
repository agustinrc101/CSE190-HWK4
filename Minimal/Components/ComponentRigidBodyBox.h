#ifndef H_COMPONENT_RIGIDBODYBOX
#define H_COMPONENT_RIGIDBODYBOX
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Physics.h"
#include "../Transform.h"
#include "Component.h"
#include "btBulletDynamicsCommon.h"

class ComponentRigidBodyBox : public Component {
public:
	bool isActive = true;

	ComponentRigidBodyBox(glm::vec3 size, bool Static = false) : colliderSize(size), isStatic(Static) {}
	~ComponentRigidBodyBox() {}

	void Init(Transform * p) {
		transform = p;
	}

	void Update(double deltaTime) override {
		update();
	}

	void LateInit() override {
		Start();
	}

protected:
	Transform * transform;
	btRigidBody * rigidbody;

	glm::vec3 colliderSize;
	bool isStatic;

	void Start() override {
		if(isStatic)
			rigidbody = transform->rigidBody = Physics::addBoxCollider(colliderSize, transform->getPosition(false), 0);
		else
			rigidbody = transform->rigidBody = Physics::addBoxCollider(colliderSize, transform->getPosition(false));
	}

	void update() {
		if (isStatic) return;

		glm::vec3 pos = bullet::ToGlm(rigidbody->getCenterOfMassPosition());
		glm::quat rot = bullet::ToGlm(rigidbody->getOrientation());

		transform->setPosition(pos, false);
		transform->setRotation(rot);
	}

};

#endif
