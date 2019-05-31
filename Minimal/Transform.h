#ifndef H_TRANSFORM
#define H_TRANSFORM
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Definitions.h"
#include "Material.h"
#include "Model.h"
#include "Components/Component.h"
#include <vector>

class Transform {
public:
	Transform * parent = 0;

	bool isActive = true;
	std::string name = "Transform";

	Transform(Model * model = NULL, Material * mat = NULL);
	~Transform();

	void draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 parent);
	void update(double deltaTime);

	//transform
	void scale(float s) { toWorld = glm::scale(toWorld, glm::vec3(s, s, s)); }
	void scale(glm::vec3 s) { toWorld = glm::scale(toWorld, s); }
	void translate(glm::vec3 t, bool localSpace = true) { 
		if(localSpace) toWorld = glm::translate(toWorld, t);		//Local space translation
		else toWorld = glm::translate(glm::mat4(1), t) * toWorld;	//Global space translation
	}
	void rotate(float angle, glm::vec3 axis) { toWorld = glm::rotate(toWorld, glm::radians(angle), axis); }

	//setters
	void setColor(glm::vec3 c)		{ if (material != NULL) material->color = c; }
	void setShader(GLuint s)		{ if (material != NULL) material->shader = s; }
	void setTextures(GLuint t)		{ if (material != NULL) material->TEX = t; }
	void setModel(Model * m)		{ model = m; }
	void setToWorld(glm::mat4 w)	{ toWorld = w; }
	void setPosition(glm::vec3 p)	{ toWorld[3] = glm::vec4(p, 1.0f); }

	//Getters
	glm::mat4 getToWorld() { return toWorld; }
	glm::mat4 getCompleteToWorld() { 
		if(parent != 0) return parent->getCompleteToWorld() * toWorld; 
		else return glm::mat4(1.0);
	}
	glm::vec3 getPosition() { return glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]) ; }
	glm::quat getRotation() { return glm::quat_cast(toWorld); }

	//Others
	void addChild(Transform * child) { children.emplace_back(child); child->parent = this; }
	void addComponent(Component * u);

private:
	glm::mat4 toWorld = glm::mat4(1);
	Material * material = NULL;
	Model * model = NULL;
	std::vector<Transform *> children;
	std::vector<std::unique_ptr<Component>> components;
};

#endif