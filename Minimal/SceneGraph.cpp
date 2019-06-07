#include "SceneGraph.h"

SceneGraph::~SceneGraph() {
	if (skybox != NULL) delete(skybox);

	for (int i = 0; i < transforms.size(); i++)
		delete(transforms[i]);
}

void SceneGraph::draw(glm::mat4 headPose, glm::mat4 projection) {
	if (isActive) {
		skybox->draw(headPose, projection, skyboxShader);

		for (int i = 0; i < transforms.size(); i++)
			transforms[i]->draw(headPose, projection, toWorld);
	}
}

void SceneGraph::update(double deltaTime) {
	if (isActive) {
		for (int i = 0; i < transforms.size(); i++)
			transforms[i]->update(deltaTime);
	}
}

void SceneGraph::addTransform(Transform * t) {
	transforms.emplace_back(t);
}
