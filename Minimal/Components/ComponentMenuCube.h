#ifndef H_COMPONENT_MENU_CUBE
#define H_COMPONENT_MENU_CUBE
#pragma once

#include <iostream>
#include "../ProjectManager.h"
#include "../Transform.h"
#include "Component.h"



class ComponentMenuCube : public Component {
public:
	bool isActive = true;

	ComponentMenuCube(ConnectionType connectionType, float collisionRadius) : type(connectionType), radius(collisionRadius) {}
	~ComponentMenuCube() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		if ((glm::distance(transform->getPosition(false), ProjectManager::project->getLeftHandPosition()) < radius) ||
			(glm::distance(transform->getPosition(false), ProjectManager::project->getRightHandPosition()) < radius)) {
			begin();
		}
	}

protected:
	Transform * transform;

	void Start() override {

	}

private:
	ConnectionType type;
	float radius;

	void begin() {
		switch (type) {
		case HOST:
			ProjectManager::project->serverConnect();
			break;
		case JOIN:
			ProjectManager::project->clientConnect(false);
			break;
		case OFFLINE:
			ProjectManager::project->offlineConnect();
			break;
		default:
			break;
		}
	}
};

#endif
