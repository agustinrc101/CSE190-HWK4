#ifndef H_PROJECT_MANAGER
#define H_PROJECT_MANAGER
#pragma once

#include <glm/glm.hpp>
#include "Definitions.h"
//Scenes
#include "SceneGraph.h"
//Sound
#include "Sounds.h"

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
	int getPlayerNumber();
	Model * getSphereModel();
	Sounds* getSoundEffect(SoundEffect soundEffect);
	
	void changeScene(Scenes scene);
	
	void offlineConnect();
	void serverConnect();
	void clientConnect(bool isHost);
	void initPacketReceived();
	void exitPacketReceived();

private:
	void initModels();
	void initObjects();
	void initSceneGraphs();
	void initGlobalScene();
	void initMenuScene();
	void initScene1();
	void initAudio();
	void initProject();

	void stopNetworking();
	void sendPlayerData();
	void receivePackets();

	void testing();
};

#endif
