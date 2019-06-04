#ifndef H_COMPONENT_RIGIDBODYSPHERE
#define H_COMPONENT_RIGIDBODYSPHERE
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Physics.h"
#include "../Transform.h"
#include "../Definitions.h"
#include "Component.h"
#include "btBulletDynamicsCommon.h"


class ComponentRigidBodySphere : public Component {
public:
	ComponentRigidBodySphere(float radius) : colliderSize(radius) {}
	~ComponentRigidBodySphere() {}

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
		rigidbody = transform->rigidBody = Physics::addSphereCollider(colliderSize, transform->getPosition(false));
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