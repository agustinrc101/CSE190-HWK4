#ifndef H_TRANSFORM
#define H_TRANSFORM
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include "btBulletDynamicsCommon.h"

#include "Definitions.h"
#include "Material.h"
#include "Model.h"
#include "Components/Component.h"

class Transform {
public:
	Transform * parent = 0;
	Material * material = NULL;
	btRigidBody * rigidBody = 0;

	bool isActive = true;
	std::string name = "Transform";

	Transform(Model * model = NULL, Material * mat = NULL, bool UseToWorld = true);
	~Transform();

	void draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 parent);
	void update(double deltaTime);

	//transform
	void scale(float s) { toWorld = glm::scale(toWorld, glm::vec3(s, s, s)); scaleMatrix = glm::scale(scaleMatrix, glm::vec3(s, s, s));  }
	void scale(glm::vec3 s) { toWorld = glm::scale(toWorld, s); scaleMatrix = glm::scale(scaleMatrix, s); }
	void translate(glm::vec3 t, bool localSpace = true) { 
		if(localSpace){		//Local space translation
			toWorld = glm::translate(toWorld, t);		
			translationMatrix = glm::translate(translationMatrix, t);
		}
		else {				//Global space translation
			toWorld = glm::translate(glm::mat4(1), t) * toWorld;
			translationMatrix = glm::translate(glm::mat4(1), t) * translationMatrix;
		}
	}
	void rotate(float angle, glm::vec3 axis) { toWorld = glm::rotate(toWorld, glm::radians(angle), axis); rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle), axis); }

	//setters
	void setColor(glm::vec3 c)		{ if (material != NULL) material->color = c; }
	void setShader(GLuint s)		{ if (material != NULL) material->shader = s; }
	void setTextures(GLuint t)		{ if (material != NULL) material->TEX = t; }
	void setModel(Model * m)		{ model = m; }
	void setToWorld(glm::mat4 w)	{ toWorld = w; }
	void setPosition(glm::vec3 p, bool localSpace = true) {
		if (localSpace) {
			toWorld[3] = glm::vec4(p, 1.0f);
			rotationMatrix[3] = glm::vec4(p, 1.0f);
		}
		else {
			if (useToWorld) {
				glm::vec3 parentGlobalPos = getPosition(false);
				parentGlobalPos = parentGlobalPos - (glm::vec3)toWorld[3];

				toWorld[3] = glm::vec4(p - parentGlobalPos, 1.0f);
			}
			else {
				glm::vec3 parentGlobalPos = getPosition(false);
				parentGlobalPos = parentGlobalPos - (glm::vec3)translationMatrix[3];

				translationMatrix[3] = glm::vec4(p - parentGlobalPos, 1.0f);
			}
		}
	}
	void setRotation(glm::quat q, bool localSpace = true) {
		if (localSpace) {
			rotationMatrix = (glm::mat4)glm::toMat4(q);
		}
		else {
			rotationMatrix = (glm::mat4)glm::toMat4(q);
		}
	}

	//Getters
	glm::mat4 getToWorld() { if (useToWorld)return toWorld; else return translationMatrix * rotationMatrix * scaleMatrix; }
	glm::mat4 getCompleteToWorld() { 
		if (parent != 0) return parent->getCompleteToWorld() * getToWorld();
		else return getToWorld();
	}
	glm::vec3 getPosition(bool localSpace = true) { 
		if (localSpace) return getToWorld()[3]; 
		else return getCompleteToWorld()[3];
	}
	glm::quat getRotation() { return glm::quat_cast(getToWorld()); }
	Transform* getCol() { return children[children.size()-1]; }
	glm::vec3 toLocalPoint(glm::vec3 v) {
		glm::vec3 parentGlobalPos = getPosition(false);
		parentGlobalPos = parentGlobalPos - (glm::vec3)getToWorld()[3];

		return (v - parentGlobalPos);
	}

	//Others
	void addChild(Transform * child) { children.emplace_back(child); child->parent = this; }
	void addComponent(Component * u);

private:
	glm::mat4 toWorld = glm::mat4(1);
	bool useToWorld = true;

	glm::mat4 translationMatrix = glm::mat4(1);
	glm::mat4 rotationMatrix = glm::mat4(1);
	glm::mat4 scaleMatrix = glm::mat4(1);

	Model * model = NULL;
	std::vector<Transform *> children;
	std::vector<std::unique_ptr<Component>> components;
};

#endif