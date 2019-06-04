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
	ComponentRigidBodyBox(glm::vec3 size) : colliderSize(size) {}
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

	void Start() override {
		rigidbody = transform->rigidBody = Physics::addBoxCollider(colliderSize, transform->getPosition(false));
	}

	void update() {
		btTransform t;

		//Gets bullet rigidbody
		rigidbody->getMotionState()->getWorldTransform(t);

		//Transforms from bullet to mat4
		glm::mat4 m = bullet::ToGlm(t);

		//transforms new position to local position
		m[3] = glm::vec4(transform->toLocalPoint(m[3]), 1);

		transform->setToWorld(m);		//TODO - Scale might be lost
	}

};

#endif
