#ifndef COMPONENT_TEST3_H
#define COMPONENT_TEST3_H

#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "Input.h"

class ComponentTest3 : public Component{
	public:
		ComponentTest3(){}
		~ComponentTest3(){}

		void Start(Transform * p) { setTransform(p); }
		
		void Update(double deltaTime){ 
			if(Input::getButtonA()){
				if(!a_press){
					a_press = true;
					print(transform->getToWorld());
				}
			}	
			else a_press = false;
		}

	protected:
		Transform * transform = 0;

	private:
		bool a_press = false;
};

#endif
