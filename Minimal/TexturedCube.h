#ifndef H_TEXTUREDCUBE
#define H_TEXTUREDCUBE
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "Material.h"

class TexturedCube{
public:
	TexturedCube(Material * mat);
	~TexturedCube();

	glm::mat4 toWorld;

	void draw(glm::mat4 projection, glm::mat4 headPose, glm::mat4 M, Material * mat);
	void update(double deltaTime);

	void setPosition(glm::vec3 pos);
	void setScale(float scale);

private:
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;

	Material * material;
	GLuint VBO, VAO, EBO, VBO2;

	void initCube(float size);
	void initBuffers();
};

#endif //TEXTURECUBE_H
