#include "SceneGraph.h"

SceneGraph::~SceneGraph() {
	for (int i = 0; i < transforms.size(); i++)
		delete(transforms[i]);
}

SceneGraph::SceneGraph(){ 
	
}

void SceneGraph::draw(glm::mat4 headPose, glm::mat4 projection) {
	for (int i = 0; i < transforms.size(); i++)
		transforms[i]->draw(headPose, projection, toWorld);
}

void SceneGraph::update(double deltaTime) {
	for (int i = 0; i < transforms.size(); i++)
		transforms[i]->update(deltaTime);
}

void SceneGraph::addTransform(Transform * t) {
	transforms.emplace_back(t);
}



