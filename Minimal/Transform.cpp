#include "Transform.h"

Transform::~Transform() {
	//Delete model if it exists
	if (model != NULL) delete(model);
	//Delete material if it exists
	if(material != NULL) delete(material);
	//Delete components
	for (int i = 0; i < components.size(); i++)	delete &components[i];
}

Transform::Transform(Model * m, Material * mat){
	//Inits variables
	model = m;
	material = mat;
}

void Transform::addComponent(Component * component){
	//Add component to list of components
	components.emplace_back(component); 
	//Calls start
	component->Init(this);
}

void Transform::draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 parent) {
	glm::mat4 M =  parent * toWorld;

	//Draw if this transform has a model
	if (model != NULL)								
		model->draw(projection, headPose, M, material);
	//Draw all of this transform's children
	for (int i = 0; i < children.size(); i++)		
		children[i]->draw(headPose, projection, M);
}

void Transform::update(double deltaTime) {
	//Update this transform's components
	for (int i = 0; i < components.size(); i++)
		components[i]->Update(deltaTime);
	//Update this transform's children
	for (int i = 0; i < children.size(); i++)
		children[i]->update(deltaTime);
}
