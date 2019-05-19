#include "Transform.h"

Transform::~Transform() {
	//Delete model if it exists
	if (model != NULL) delete(model);

	//Delete components
	for (int i = 0; i < components.size(); i++)	delete &components[i];
}

Transform::Transform(Model * m, GLint s, glm::vec3 c){
	//Inits variables
	model = m;
	shader = s;
	color = c;
}

void Transform::addComponent(Component * component){
	//Add component to list of components
	components.emplace_back((std::unique_ptr<Component>)component); 
	//Calls start
	component->Start(this); 
}

void Transform::draw(glm::mat4 headPose, glm::mat4 projection) {
	//Draw of this transform has a component
	if (model != NULL) model->draw(projection, headPose, shader, color, toWorld);
}

void Transform::update(double deltaTime) {
	//Calls update in every component
	for (int i = 0; i < components.size(); i++) ((Component *)(&components[i]))->Update(deltaTime);
}

