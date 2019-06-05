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
	ComponentRigidBodyBox(glm::vec3 size, float Mass = 1.0f) : colliderSize(size), mass(Mass) {}
	~ComponentRigidBodyBox() {}

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

	glm::vec3 colliderSize;
	float mass;

	void Start() override {
		rigidbody = transform->rigidBody = Physics::addBoxCollider(colliderSize, transform->getPosition(false), mass);
	}

	void update() {
		glm::vec3 pos = bullet::ToGlm(rigidbody->getCenterOfMassPosition());
		glm::quat rot = bullet::ToGlm(rigidbody->getOrientation());

		transform->setPosition(pos, false);
		transform->setRotation(rot);
	}

};

#endif
