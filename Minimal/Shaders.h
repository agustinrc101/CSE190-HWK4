#ifndef SHADERS_H
#define SHADERS_H
#pragma once

#include <GL/glew.h>
#include "Shader.h"

class Shaders{
public:
	//Init
	static void init() {
		Shaders::setColorShader(LoadShaders(SHADER_COLOR_VERTEX, SHADER_COLOR_FRAGMENT));
		Shaders::setTextureShader(LoadShaders(SHADER_TEXTURE_VERTEX, SHADER_TEXTURE_FRAGMENT));
		Shaders::setSkyboxShader(LoadShaders(SHADER_SKYBOX_VERTEX, SHADER_SKYBOX_FRAGMENT));
		Shaders::setGradientShader(LoadShaders(SHADER_GRADIENT_VERTEX, SHADER_GRADIENT_FRAGMENT));
	}

	//Setters
	static void setColorShader(GLint s) {colorShader = s;}
	static void setTextureShader(GLint s) { textureShader = s;}
	static void setSkyboxShader(GLint s) { skyboxShader = s; }
	static void setGradientShader(GLint s) { gradientShader = s; }

	//Getters
	static GLint getColorShader() { return colorShader; }
	static GLint getTextureShader() { return textureShader; }
	static GLint getSkyboxShader() { return skyboxShader; }
	static GLint getGradientShader() { return gradientShader; }

	//delete shaders
	static void deleteShaders(){
		glDeleteProgram(colorShader);
		glDeleteProgram(textureShader);
		glDeleteProgram(skyboxShader);
		glDeleteProgram(gradientShader);
	}

protected:
	static GLint colorShader; 
	static GLint textureShader;
	static GLint skyboxShader;
	static GLint gradientShader;
};

#endif