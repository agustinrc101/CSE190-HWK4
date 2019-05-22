#ifndef H_SCENEGRAPH
#define H_SCENEGRAPH
#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Skybox.h"
#include "Transform.h"

class SceneGraph{
public:
	bool isActive = true;

	SceneGraph(Skybox * sb, GLuint sbShader) : skybox(sb), skyboxShader(sbShader) { }
	~SceneGraph();

	void draw(glm::mat4 headPose, glm::mat4 projection);
	void update(double deltaTime);

	void addTransform(Transform * t);

private:
	Skybox * skybox;
	GLuint skyboxShader;
	std::vector<Transform *> transforms;
	glm::mat4 toWorld = glm::mat4(1);
};

#endif
