#ifndef H_COMPONENT_TEST1
#define H_COMPONENT_TEST1
#pragma once

#include <iostream>
#include "Transform.h"
#include "Component.h"


class ComponentTest1 : public Component{
	public:
		ComponentTest1(){}
		~ComponentTest1(){}

		void Init(Transform * p) { 
			transform = p;
			Start();
		}
		
		void Update(double deltaTime) override { 
			
		}

	protected:
		Transform * transform;

		void Start() override {
			std::cout << "ComponentTest1 Start" << std::endl;
		}
};

#endif
