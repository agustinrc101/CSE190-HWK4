#ifndef H_MODEL
#define H_MODEL
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Material.h"

class Model{
public:
	Model(const char * path);
	Model(const Model& model);
	~Model();

	void draw(glm::mat4 projection, glm::mat4 headPose, glm::mat4 M, Material * mat);

private:
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	GLuint VBO, VAO, EBO, VBO2;

	void parse(const char * filepath);
	void initBuffers();
};

#endif
