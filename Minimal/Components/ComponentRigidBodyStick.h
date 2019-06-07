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
	ComponentRigidBodyStick(float size) : colliderSize(size) {}
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
	std::vector<glm::vec3> allPos;
	glm::vec3 lastPos = glm::vec3(0);
	glm::vec3 tempVelocity = glm::vec3(0);

	bool firstFrame = true;
	float colliderSize;

	void Start() override {
		rigidbody = transform->rigidBody = Physics::addStickCollider(colliderSize, transform->getPosition(false));
	}



	glm::vec3 getVelocity() {						//CALCULATES VELOCITY (POSITION OVER A SECOND



		if ((clock() / CLOCKS_PER_SEC) * 10.0f > up) {
			//std::cout << up << endl;
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



		/*average /= 10;

		glm::vec3 distance = positions[0] - positions[9];*/

		return tempVelocity;

	}

	void update() {
	

		velo = getVelocity();								//GETS VELOCITY
		if (!firstFrame)										//GETS LAST FRAME
			lastPos = transform->getPosition(false);

		firstFrame = false;

		//cout << velo.x << ", " << velo.y << ", " << velo.z << ", " << endl;
	}

};

#endif
