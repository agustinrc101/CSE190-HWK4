#ifndef H_COMPONENT_TEST3
#define H_COMPONENT_TEST3
#pragma once

#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "Input.h"

class ComponentTest3 : public Component{
	public:
		ComponentTest3(bool leftHand) { left = leftHand; }
		~ComponentTest3(){}

		void Init(Transform * p) { 
			transform = p;
			Start();
		}
		
		void Update(double deltaTime) override { 
			if (left) leftController();
			else rightController();
		}

	protected:
		Transform * transform = 0;

		void Start() override {
			std::cout << "ComponentTest3 Start" << std::endl;
		}

	private:
		bool left = false;
		bool button_press = false;

		void leftController() {
			if (Input::getButtonX()) {
				if (!button_press) {
					button_press = true;
					print(transform->getPosition());
				}
			}
			else button_press = false;
		}

		void rightController() {
			if (Input::getButtonA()) {
				if (!button_press) {
					button_press = true;
					print(transform->getPosition());
				}
			}
			else button_press = false;
		}
};

#endif
