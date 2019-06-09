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
	glm::vec3 velo = glm::vec3(0);
	bool touchingGround = false;
  
	void Init(Transform * p) {
		transform = p;
		Start();
		
	}

	void Update(double deltaTime) override {
		update(deltaTime);
	}

	glm::vec3 getlinVelo() {
		//velo.y = 0.1f;
		velo = 0.7f * 10.0f * velo;
	
		return velo;
	}
protected:
	Transform * transform;
	btRigidBody * rigidbody;

	bool left;
	glm::vec3 colliderSize;

	void Start() override {

		lastPos = glm::vec3(0);

		rigidbody = transform->rigidBody = Physics::addStickCollider(colliderSize, transform->getPosition(), left);

		if(left)
			rigidbody->getCollisionShape()->setUserIndex(LAYER_STICK_LEFT);
		else
			rigidbody->getCollisionShape()->setUserIndex(LAYER_STICK_RIGHT);
	}


private:
	std::vector<glm::vec3> allPos;
	
	glm::vec3 lastPos = glm::vec3(0); 
	glm::vec3 tempVelocity = glm::vec3(1);
	
	bool firstFrame = true;
	float curTime = 0.f;

	glm::vec3 getVelocity(double deltaTime) {						//CALCULATES VELOCITY (POSITION OVER A SECOND
		curTime += (float)deltaTime;
		if(curTime >= 0.1f){
			curTime = 0;
			up+=10;
			tempVelocity = glm::vec3(0);
			
			for (int i = 0; i < allPos.size(); i++) {

				tempVelocity += allPos[i];
			}
			allPos.clear();
			allPos.push_back(glm::vec3(bullet::ToGlm(rigidbody->getWorldTransform())[3]) - lastPos);
		}
		else {
			allPos.push_back(glm::vec3(bullet::ToGlm(rigidbody->getWorldTransform())[3]) - lastPos);
		}
		
		return tempVelocity;
		

	}

	void update(double deltaTime) {
	

		velo = getVelocity(deltaTime);								//GETS VELOCITY
	
		if (!firstFrame)										//GETS LAST FRAME
			lastPos = bullet::ToGlm(rigidbody->getWorldTransform())[3];
			//lastPos = transform->getPosition(false);

		
		
		firstFrame = false;

		
	}

};

#endif
