#ifndef H_COMPONENT_PRINT_POSITION
#define H_COMPONENT_PRINT_POSITION
#pragma once

#include <iostream>
#include "../Transform.h"
#include "Component.h"
#include "Input.h"

class ComponentPrintPosition : public Component{
	public:
		bool isActive = true;

		ComponentPrintPosition() {}
		~ComponentPrintPosition(){}

		void Init(Transform * p) { 
			transform = p;
			Start();
		}
		
		void Update(double deltaTime) override { 
			if (Input::getButtonA()) {
				if (!button_press) {
					button_press = true;
					std::cout << transform->name << "'s position: ";
					print(transform->getPosition());
				}
			}
			else button_press = false;
		}

	protected:
		Transform * transform = 0;

		void Start() override {
			
		}

	private:
		bool button_press = false;
};

#endif
