#ifndef H_QUAD
#define H_QUAD
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Material.h"

class Quad{
public:
	Quad(float size);
	~Quad();
	
	glm::mat4 toWorld = glm::mat4(1.0f);
	std::vector<glm::vec3> vertices;

	void draw(glm::mat4 projection, glm::mat4 headPose,glm::mat4 M, Material * mat);
	void update();

private:
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;

	GLuint VBO, VAO, EBO, VBO2;

	void initPlane(float size);
	void initBuffers();
};

#endif