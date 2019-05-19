#ifndef H_LINES
#define H_LINES
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Material.h"

class Lines{
public:
	Lines() { }
	Lines(Material * mat) : material(mat) { }
	~Lines();

	glm::mat4 toWorld = glm::mat4(1.0f);

	void draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 M, Material * mat);
	void draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 M);
	void updateInitialPosition(glm::vec3 pos);
	void addVertex(glm::vec3 v);

private:
	Material * material = NULL;
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	GLuint VBO, VAO, EBO, VBO2;

	void bindBuffers();
};

#endif

