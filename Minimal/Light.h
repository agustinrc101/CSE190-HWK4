#ifndef H_LIGHT
#define H_LIGHT
#pragma once

#include <glm/glm.hpp>

class Light {
public:
	//init
	static void init(glm::vec3 position, glm::vec3 color) {
		Light::setLightPosition(position);
		Light::setLightColor(color);
	}

	//Setters
	static void setLightPosition(glm::vec3 p) { position = p; }
	static void setLightColor(glm::vec3 c) { color = c; }
	static void setCameraPos(glm::vec3 cp) { cameraPos = cp; }
	//Getters
	static glm::vec3 getLightPosition() { return position; }
	static glm::vec3 getLightColor() { return color; }
	static glm::vec3 getCameraPos() { return cameraPos; }

protected:
	static glm::vec3 position;
	static glm::vec3 color;
	static glm::vec3 cameraPos;
};

#endif