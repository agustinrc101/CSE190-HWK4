#ifndef H_COMPONENT_RIGIDBODYSPHERE
#define H_COMPONENT_RIGIDBODYSPHERE
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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
		glm::vec3 pos = bullet::ToGlm(rigidbody->getCenterOfMassPosition());
		glm::quat rot = bullet::ToGlm(rigidbody->getOrientation());

		transform->setPosition(pos, false);
		transform->setRotation(rot);
	}

};

#endif