#pragma once
#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <glm/glm.hpp>

class ProjectManager {
public:
	ProjectManager();
	~ProjectManager();

	void draw(glm::mat4 headPose, glm::mat4 projection, int eye);
	void update(double deltaTime);
	void updateHands(glm::mat4 handL, glm::mat4 handR);

private:
	void initShaders();
	void initTextures();
	void initModels();
	void initObjects();
	void initValues();
};

#endif
