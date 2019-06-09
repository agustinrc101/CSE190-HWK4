#ifndef H_COMPONENT_MENU_CUBE
#define H_COMPONENT_MENU_CUBE
#pragma once

#include <iostream>
#include "../ProjectManager.h"
#include "../Transform.h"
#include "Component.h"



class ComponentMenuCube : public Component {
public:
	ComponentMenuCube(bool host, float collisionRadius) : isHosting(host), radius(collisionRadius) {}
	~ComponentMenuCube() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		if ((glm::distance(transform->getPosition(false), ProjectManager::project->getLeftHandPosition()) < radius) ||
			(glm::distance(transform->getPosition(false), ProjectManager::project->getRightHandPosition()) < radius)) {
			beginNetwork();
		}
	}

protected:
	Transform * transform;

	void Start() override {

	}

private:
	bool isHosting;
	float radius;

	void beginNetwork() {
		if (isHosting) ProjectManager::project->serverConnect();
		else ProjectManager::project->clientConnect(false);
	}
};

#endif
