#ifndef TEXTURES_H
#define TEXTURES_H
#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "LoadPPM.h"
//#define STB_IMAGE_IMPLEMENTATION	//needs to be called ONCE before the first #include "stbi.image.h"
#include "stb_image.h"

class Textures{
	public:
		//Getters
		static void setTextureSkybox(GLuint t){ textureSkybox = t; }
		static void setTextureSteam(GLuint t) { textureSteam = t; }
		static void setTextureGrip1Albedo(GLuint t) { textureGrip1Albedo = t; }
		static void setTextureGrip2Albedo(GLuint t) { textureGrip2Albedo = t; }
		static void setTextureTrash(GLuint t) { textureTrash = t; }

		//Setters
		static GLuint getTextureSkybox(){ return textureSkybox; }
		static GLuint getTextureSteam() { return textureSteam; }
		static GLuint getTextureGrip1Albedo() { return textureGrip1Albedo; }
		static GLuint getTextureGrip2Albedo() { return textureGrip2Albedo; }
		static GLuint getTextureTrash() { return textureTrash; }

		//Delete textures
		static void deleteTextures() {
			glDeleteTextures(1, &textureSkybox);
			glDeleteTextures(1, &textureSteam);
			glDeleteTextures(1, &textureGrip1Albedo);
			glDeleteTextures(1, &textureGrip2Albedo);
			glDeleteTextures(1, &textureTrash);
		}

	protected:
		static GLuint textureSkybox;
		static GLuint textureSteam;
		static GLuint textureGrip1Albedo;
		static GLuint textureGrip2Albedo;
		static GLuint textureTrash;

};

static GLuint LoadTextures(const char * path) {
	std::cout << "Loading textures: " << path << std::endl;
	GLuint TEX = 0;
	int width = 0;
	int height = 0;
	int nrcChannels = 0;
	unsigned char * ppm = stbi_load(path, &width, &height, &nrcChannels, 0);

	if (!ppm) {
		std::cout << "\tTexture failed to load" << std::endl;
		delete(ppm);
		return 0;
	}

	glGenTextures(1, &TEX);
	
	GLenum format;
	if (nrcChannels == 1)
		format = GL_RED;
	else if (nrcChannels == 3)
		format = GL_RGB;
	else if (nrcChannels == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, TEX);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, &ppm[0]);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	delete(ppm);

	return TEX;
}

static GLuint LoadCubeMap(std::string path) {
	std::vector<std::string> faces =
	{ path + "/left.ppm",		//left		//px
		path + "/right.ppm",	//right		//nx
		path + "/up.ppm",		//up		//py
		path + "/down.ppm",		//down		//ny
		path + "/back.ppm",		//back		//pz
		path + "/front.ppm"		//front		//nz
	};

	GLuint TEX = 0;

	glGenTextures(1, &TEX);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEX);

	int width, height, nrcChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		
		switch (i) {
		case 0:
			std::cout << "\tLoading right texture...\n";
			break;
		case 1:
			std::cout << "\tLoading left texture...\n";
			break;
		case 2:
			std::cout << "\tLoading top texture...\n";
			break;
		case 3:
			std::cout << "\tLoading bottom texture...\n";
			break;
		case 4:
			std::cout << "\tLoading back texture...\n";
			break;
		case 5:
			std::cout << "\tLoading front texture...\n";
			break;
		default:
			std::cout << "\tERROR - invalid texture index\n";
			break;
		}

		unsigned char * data = stbi_load(faces[i].c_str(), &width, &height, &nrcChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "\tCubemap texture failed to load at path: " << faces[i].c_str() << "\n";
		}
		stbi_image_free(data);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return TEX;
}

#endif
