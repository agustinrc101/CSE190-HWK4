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
	glm::vec3 color2;
	float specular;
	float ambiance;

	//Constructor/Destructor
	Material(GLuint Shader = 0, glm::vec3 Color =glm::vec3(COLOR_WHITE),  GLuint Tex=0) 
	: shader(Shader), TEX(Tex), color(Color), ambiance(0.1f), specular(0.5f) { 
		color2 = glm::vec3(COLOR_WHITE);
	}

	~Material() {}

	void setAmbiance(float Ambiance){
		if (Ambiance > 1.0f) ambiance = 1.0f;
		else if (Ambiance < 0.0f) ambiance = 0.0f;
		else ambiance = Ambiance;
	}

	void setSpecular(float Specular){
		if (Specular > 1.0f) specular = 1.0f;
		else if (Specular < 0.0f) specular = 0.0f;
		else specular = Specular;
	}
};

#endif
