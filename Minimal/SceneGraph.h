#ifndef H_SCENEGRAPH
#define H_SCENEGRAPH
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Transform.h"

class SceneGraph{
public:
	SceneGraph();
	~SceneGraph();

	void draw(glm::mat4 headPose, glm::mat4 projection);
	void update(double deltaTime);

	void addTransform(Transform * t);

private:
	std::vector<Transform *> transforms;

};

#endif
