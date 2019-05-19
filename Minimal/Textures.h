#ifndef TEXTURES_H
#define TEXTURES_H

#include <GL/glew.h>
#include "LoadPPM.h"

class Textures{
	public:
		//Getters
		static void setTextureSteam(GLuint t){ textureSteam = t; }

		//Setters
		static GLuint getTextureSteam(){ return textureSteam; }

	protected:
		static GLuint textureSteam;

};

static GLuint LoadTextures(const char * path) {
	GLuint TEX = 0;
	int width = 0;
	int height = 0;
	int nrcChannels = 0;
	unsigned char * ppm = loadPPM(path, width, height);

	glGenTextures(1, &TEX);
	glBindTexture(GL_TEXTURE_2D, TEX);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, &ppm[0]);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

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

#endif
