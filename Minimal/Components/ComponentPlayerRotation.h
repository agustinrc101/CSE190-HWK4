#ifndef H_COMPONENT_PLAYER_ROTATION
#define H_COMPONENT_PLAYER_ROTATION
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"


class ComponentPlayerRotation : public Component {
public:
	bool isActive = true;

	ComponentPlayerRotation() {}
	~ComponentPlayerRotation() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		//playerRotation();
	}

protected:
	Transform * transform;

	void Start() override {
		
	}

private:
	bool button_press = false;

	void playerRotation() {
		if (Input::getButtonY() || Input::getButtonB()) {
			if (!button_press) {
				button_press = true;
				transform->rotate(90, AXIS_Y_NEGATIVE);
			}
		}
		else if (Input::getButtonX() || Input::getButtonA()) {
			if (!button_press) {
				button_press = true;
				transform->rotate(90, AXIS_Y_POSITIVE);
			}
		}
		else {
			button_press = false;
		}
	}
};

#endif