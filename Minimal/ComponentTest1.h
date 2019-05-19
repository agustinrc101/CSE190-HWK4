#ifndef COMPONENT_TEST1_H
#define COMPONENT_TEST1_H

#include <iostream>
#include "Transform.h"
#include "Component.h"

class ComponentTest1 : public Component{
	public:
		ComponentTest1(){}
		~ComponentTest1(){ std::cout << "DELETING Component Test 1" << std::endl; }

		void Start(Transform * p) { setTransform(p); std::cout << "START Component Test 1" << std::endl; }
		void Update(double deltaTime) { std::cout << "UPDATE Component Test 1" << std::endl; }

	protected:
		Transform * transform;
};

#endif
