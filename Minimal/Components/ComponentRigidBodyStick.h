#ifndef H_COMPONENT_RIGIDBODYSTICK
#define H_COMPONENT_RIGIDBODYSTICK
#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../Physics.h"
#include "../Transform.h"
#include "Component.h"
#include "btBulletDynamicsCommon.h"
#include <ctime>


class ComponentRigidBodyStick : public Component {
public:
	ComponentRigidBodyStick(glm::vec3 size, bool leftHand) : colliderSize(size), left(leftHand) {}
	~ComponentRigidBodyStick() {}
	int up = 0;
	glm::vec3 velo;
  
	void Init(Transform * p) {
		transform = p;
		Start();
		
	}

	void Update(double deltaTime) override {
		update();
	}

	glm::vec3 getlinVelo() {
		velo.y = 0.01f;
		velo = 10.0f * 10.0f * velo;

		return velo;
	}
protected:
	Transform * transform;
	btRigidBody * rigidbody;

	bool left;
	glm::vec3 colliderSize;

	void Start() override {

		lastPos = transform->getPosition(false);

		rigidbody = transform->rigidBody = Physics::addStickCollider(colliderSize, transform->getPosition(), left);

		if(left)
			rigidbody->getCollisionShape()->setUserIndex(LAYER_STICK_LEFT);
		else
			rigidbody->getCollisionShape()->setUserIndex(LAYER_STICK_RIGHT);
	}


private:
	std::vector<glm::vec3> allPos;
	glm::vec3 positions[10] = {glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0),glm::vec3(0)};
	glm::vec3 lastPos = glm::vec3(0); 
	glm::vec3 tempVelocity = glm::vec3(0);
	
	bool firstFrame = true;

	glm::vec3 getVelocity() {						//CALCULATES VELOCITY (POSITION OVER A SECOND
		if ((clock() / CLOCKS_PER_SEC) * 10.0f > up) {
			up++;
			tempVelocity = glm::vec3(0);
			for (int i = 0; i < allPos.size(); i++) {

				tempVelocity += allPos[i];
			}
			allPos.clear();
		}
		else {
			allPos.push_back(transform->getPosition(false) - lastPos);
		}

		return tempVelocity;

	}

	void update() {
	

		velo = getVelocity();								//GETS VELOCITY
		if (!firstFrame)										//GETS LAST FRAME
			lastPos = transform->getPosition(false);

		firstFrame = false;

		cout << velo.x << ", " << velo.y << ", " << velo.z << ", " << endl;
	}

};

#endif
