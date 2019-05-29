#ifndef H_PROJECT_MANAGER
#define H_PROJECT_MANAGER
#pragma once

#include <glm/glm.hpp>

class ProjectManager {
public:
	ProjectManager();
	~ProjectManager();

	void draw(glm::mat4 headPose, glm::mat4 projection, int eye);
	void update(double deltaTime);
	void updateHands(glm::mat4 handL, glm::mat4 handR);
	void updateHead(glm::mat4 eye);

private:
	void initShadersAndLighting();
	void initTextures();
	void initModels();
	void initObjects();
	void initSceneGraphs();
	void initGlobalScene();
	void initMenuScene();
	void initScene1();
	void initAudio();
	void initProject();

	void networkingSetup();
	void initNetworking();
	void stopNetworking();

	void testing();
};

#endif
