#ifndef COMPONENT_TEST2_H
#define COMPONENT_TEST2_H

#include <iostream>
#include "Transform.h"
#include "Component.h"

class ComponentTest2 : public Component {
public:
	ComponentTest2() {}
	~ComponentTest2() { std::cout << "DELETING Component Test 2" << std::endl; }

	void Start(Transform * p) { setTransform(p); std::cout << "START Component Test 2" << std::endl; }
	void Update(double deltaTime) { std::cout << "UPDATE Component Test 2" << std::endl; }

protected:
	Transform * transform;
};

#endif