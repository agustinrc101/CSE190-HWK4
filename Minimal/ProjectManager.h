#ifndef H_PROJECT_MANAGER
#define H_PROJECT_MANAGER
#pragma once

#include <glm/glm.hpp>

class ProjectManager {
public:
	static ProjectManager * project;

	ProjectManager();
	~ProjectManager();

	void draw(glm::mat4 headPose, glm::mat4 projection, int eye);
	void update(double deltaTime);
	void updateHands(glm::mat4 handL, glm::mat4 handR);
	void updateHead(glm::mat4 hmd);
	void updateLightCameraPos(glm::mat4 eye);

	glm::vec3 getPlayerPosition();
	glm::quat getPlayerRotation();
	glm::mat4 getPlayerToWorld();
	glm::vec3 getLeftHandPosition();
	glm::vec3 getRightHandPosition();
	glm::vec3 getStickVelocity(bool left);
	

private:
	void initBulletPhysics();
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
	void serverConnect();
	void clientConnect();
	void stopNetworking();
	void sendPlayerData();
	void receivePackets();

	void testing();
};

#endif
