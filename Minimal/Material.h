#ifndef H_MATERIAL
#define H_MATERIAL
#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "Definitions.h"

class Material {
public:
	//Vars
	GLuint shader;
	GLuint TEX;
	glm::vec3 color;

	//Constructor/Destructor
	Material(GLuint Shader = 0, glm::vec3 Color =glm::vec3(COLOR_WHITE),  GLuint tex=0) 
		: shader(Shader), TEX(tex), color(Color) {}

	~Material() {}
};

#endif
