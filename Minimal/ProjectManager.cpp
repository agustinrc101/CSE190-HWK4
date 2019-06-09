#define STB_IMAGE_IMPLEMENTATION

#include "ProjectManager.h"

//Shaders and textures
#include "Shaders.h"
#include "Light.h"
#include "Textures.h"
#include "Material.h"
//Rendering
#include "SceneGraph.h"
#include "Model.h"
#include "Transform.h"
#include "TexturedCube.h"
#include "Skybox.h"
#include "Lines.h"
//Components
#include "Components/ComponentHeader.h"	//Includes all component scripts
//Networking
#include "Networking/Server.h"
#include "Networking/Client.h"

//Physics
#include "Physics.h"

//Init Shaders
GLint Shaders::colorShader = 0;
GLint Shaders::textureShader = 0;
GLint Shaders::skyboxShader = 0;
//Init Light
glm::vec3 Light::position = glm::vec3(0);
glm::vec3 Light::color = glm::vec3(COLOR_WHITE);
glm::vec3 Light::cameraPos = glm::vec3(0);

//Init Networking
Client * Client::client = 0;
Client * client;
bool startedNetwork = false;
bool Server::serverOn = false;
//Init Physics
Physics * Physics::physics = 0;

//Init Sound
Sounds * sound_BG;
Sounds * sound_Hit;
//Physics
Physics * physics;

//Init SceneGraph
SceneGraph * sceneGlobal;
SceneGraph * sceneMenu;
SceneGraph * scene1;
//Declare Models
Model * model_sphere;
Model * model_cube;
Model * model_plane;
Model * model_stick;
Model * model_robot;
Model * model_hand_left;
Model * model_hand_right;
Model * model_goal;
//Declare Important Objects
Transform * player;
Transform * head;
Transform * handL;
Transform * handR;
Transform * handLModel;
Transform * handRModel;
Transform * otherHead;
Transform * otherHandL;
Transform * otherHandR;
Transform * ball;
//Components
glm::vec3 groundPosition = glm::vec3(0, -1.7f, 0);
float stageSize = 10.0f;
ComponentRigidBodyStick * stickL;
ComponentRigidBodyStick * stickR;
//Scene Management
Scenes curScene = SCENE_MENU;

//Debug
Lines * lines;

ProjectManager::~ProjectManager() {
	//Stop Server and client
	Server::stopServer();
	delete(client);

	//Delete SceneGraph	(also deletes transforms, models, and components) --> make sure to use every declared model/component/transform
	delete(sceneGlobal);
	delete(sceneMenu);
	delete(scene1);
	//Delete lines
	delete(lines);
	//Delete shaders
	Shaders::deleteShaders();
	//Delete textures
	Textures::deleteTextures();
	delete(sound_BG);
	delete(sound_Hit);
	delete(physics);
}

ProjectManager::ProjectManager() {
	project = this;

	physics = new Physics();
	Light::init(glm::vec3(-10, 10, 5), glm::vec3(COLOR_WHITE));
	Shaders::init();
	Textures::init();
	initModels();
	initObjects();
	initSceneGraphs();
	initAudio();
	initProject();
	client = new Client();
}

void ProjectManager::initModels() {
	model_sphere = new Model(MODEL_SPHERE);
	model_cube = new Model(MODEL_CUBE);
	model_plane = new Model(MODEL_PLANE);
	model_stick = new Model(MODEL_STICK);
	model_robot = new Model(MODEL_ROBOT);
	model_hand_left = new Model(MODEL_HAND_LEFT);
	model_hand_right = new Model(MODEL_HAND_RIGHT);
	model_goal = new Model(MODEL_GOAL);
}

void ProjectManager::initObjects() {
	//Initialize non-scenegraph objects here
	lines = new Lines(new Material(Shaders::getColorShader(), glm::vec3(COLOR_GREEN)));
}

void ProjectManager::initSceneGraphs() {
	initGlobalScene();
	initMenuScene();
	initScene1();
}

void ProjectManager::initGlobalScene() {
	sceneGlobal = new SceneGraph(new Skybox(), 0);	//No parameters on Skybox and a 0 don't render a skybox
	sceneGlobal->isActive = true;

	glm::vec3 stickSize = glm::vec3(0.075f, 1.0f, 0.075f);
	Transform * colliderR;
	Transform * colliderL;

	//==================================
	//Initialize sceneGlobal objects here
	//==================================
	//Right hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
		handR = new Transform(model_sphere, mat);
		handR->name = "Right Hand";

		mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTexture(Textures::T_STICK));
		Transform * stick = new Transform(model_stick, mat);

		stick->scale(glm::vec3(0.5f, 0.7f, 0.5f));
		stick->translate(glm::vec3(0, -175, 0));

		stickR = new ComponentRigidBodyStick(stickSize, false);
		handR->addComponent(stickR);

		colliderR = new Transform();

		colliderR->scale(5);
		colliderR->translate(glm::vec3(0, -20, 0));

		handR->addChild(stick);
		handR->addChild(colliderR);

	}
	//Left Hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_RED));
		handL = new Transform(model_sphere, mat);
		handL->name = "Left Hand";

		mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTexture(Textures::T_STICK));
		Transform * stick = new Transform(model_stick, mat);

		stick->scale(glm::vec3(0.5f, 0.7f, 0.5f));
		stick->translate(glm::vec3(0, -175, 0));

		stickL = new ComponentRigidBodyStick(stickSize, true);
		handL->addComponent(stickL);

		colliderL = new Transform();

		colliderL->scale(5);
		colliderL->translate(glm::vec3(0, -20, 0));

		handL->addChild(stick);
		handL->addChild(colliderL);
	}
	//Head setup
	{
		head = new Transform();
		head->name = "Head";
	}
	//Hand models
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_RED), Textures::getTexture(Textures::T_GRIP2));
		handLModel = new Transform(model_hand_left, mat);
		handRModel = new Transform(model_hand_right, mat);
	}
	//Player parent
	{
		//Create transform
		Material * mat = new Material();
		player = new Transform();
		player->name = "Player";
		//Add children
		player->addChild(head);
		player->addChild(handL);
		player->addChild(handR);
		player->addChild(handLModel);
		player->addChild(handRModel);

		//Pogo Movement
		ComponentPogoMovement * c1 = new ComponentPogoMovement(head, handR, colliderR, stickR);
		player->addComponent(c1);
		c1 = new ComponentPogoMovement(head, handL, colliderL, stickL);
		player->addComponent(c1);
		//Player Rotation
		ComponentPlayerRotation * c2 = new ComponentPlayerRotation();
		player->addComponent(c2);
		//Add to scene
		sceneGlobal->addTransform(player);
	}

	//Other player setup
	//Right hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_WHITE));
		otherHandR = new Transform(model_sphere, mat);
		otherHandR->name = "Other Player's Right Hand";
		otherHandR->isActive = false;
		sceneGlobal->addTransform(otherHandR);

	}
	//Left Hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_WHITE));
		otherHandL = new Transform(model_sphere, mat);
		otherHandL->name = "Other Player's Left Hand";
		otherHandL->isActive = false;
		sceneGlobal->addTransform(otherHandL);
	}
	//Head setup
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTexture(Textures::T_ROBOT));
		otherHead = new Transform(model_robot, mat);
		otherHead->name = "Other Player's Head";
		otherHead->isActive = false;

		sceneGlobal->addTransform(otherHead);
	}

	sceneGlobal->LateInit();
}

void ProjectManager::initMenuScene() {
	sceneMenu = new SceneGraph(new Skybox(), 0);	//No parameters on Skybox and a 0 don't render a skybox

	//==================================
	//Initialize sceneMenu objects here
	//==================================
	{

	}

	sceneMenu->LateInit();
}

void ProjectManager::initScene1() {
	scene1 = new SceneGraph(new Skybox(Textures::getTexture(Textures::T_SKYBOX)), Shaders::getSkyboxShader());
	//==================================
	//Initialize scene1 objects here
	//==================================
	//Gound
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTexture(Textures::T_GRASS));
		Transform * transform = new Transform(model_plane, mat);

		transform->translate(groundPosition);	//ACTUAL GROUND POS
		//transform->translate(glm::vec3(0, -.4f, 0));	//DEBUG GROUND POS
		
		ComponentRigidBodyPlane * col = new ComponentRigidBodyPlane(stageSize * 2);
		transform->addComponent(col);

		scene1->addTransform(transform);
	}
	//Walls
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_GRAY), Textures::getTexture(Textures::T_WINDOW));
		Transform * transform = new Transform(model_plane, mat);
		transform->translate(glm::vec3(-stageSize, 0, 0));
		transform->rotate(90, AXIS_Z_POSITIVE);
		transform->rotate(90, AXIS_Y_POSITIVE);
		ComponentRigidBodyPlane * plane = new ComponentRigidBodyPlane(stageSize * 2, AXIS_X_NEGATIVE);
		transform->addComponent(plane);
		scene1->addTransform(transform);

		transform = new Transform(model_plane, mat);
		transform->translate(glm::vec3(stageSize, 0, 0));
		transform->rotate(-90, AXIS_Z_POSITIVE);
		transform->rotate(90, AXIS_Y_POSITIVE);
		plane = new ComponentRigidBodyPlane(stageSize * 2, AXIS_X_POSITIVE);
		transform->addComponent(plane);
		scene1->addTransform(transform);

		transform = new Transform(model_plane, mat);
		transform->translate(glm::vec3(0, 0, stageSize));
		transform->rotate(-90, AXIS_X_POSITIVE);
		plane = new ComponentRigidBodyPlane(stageSize * 2, AXIS_Z_NEGATIVE);
		transform->addComponent(plane);
		scene1->addTransform(transform);

		transform = new Transform(model_plane, mat);
		transform->translate(glm::vec3(0, 0, -stageSize));
		transform->rotate(90, AXIS_X_POSITIVE);
		plane = new ComponentRigidBodyPlane(stageSize * 2, AXIS_Z_POSITIVE);
		transform->addComponent(plane);
		scene1->addTransform(transform);
	}
	//Ball
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_CYAN), Textures::getTexture(Textures::T_STEAM));
		ball = new Transform(model_sphere, mat, false);

		ball->scale(0.4f);

		//transform->translate(glm::vec3(-2, 10, -0.8f));
		ball->translate(glm::vec3(-1.5, 0, -0.7f));

		ComponentRigidBodySphere * col = new ComponentRigidBodySphere(0.4f);
		ball->addComponent(col);
		ComponentBallProperties * c1 = new ComponentBallProperties(stickL, stickR);
		ball->addComponent(c1);

		scene1->addTransform(ball);
	}
	//Goalpost - RED
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_RED));
		Transform * goal = new Transform(model_goal, mat);

		goal->translate(glm::vec3(0, 0, 7.5) + groundPosition);
		goal->rotate(180, AXIS_Y_POSITIVE);

		{
			float colThicc = 0.05f;

			Transform * child = new Transform();
			child->translate(glm::vec3(-2.f, 1.25f, -0.5f));
			child->scale(0.25f);
			goal->addChild(child);
			ComponentRigidBodyBox * c = new ComponentRigidBodyBox(glm::vec3(colThicc, 1.25f, 0.5f), true);
			child->addComponent(c);


			child = new Transform();
			child->translate(glm::vec3(2.f, 1.25f, -0.5f));
			child->scale(0.25f);
			goal->addChild(child);
			c = new ComponentRigidBodyBox(glm::vec3(colThicc, 1.25f, 0.5f), true);
			child->addComponent(c);

			child = new Transform();
			child->translate(glm::vec3(0.f, 2.5f, -0.5f));
			child->scale(0.25f);
			goal->addChild(child);
			c = new ComponentRigidBodyBox(glm::vec3(2.f, colThicc, 0.5f), true);
			child->addComponent(c);

			child = new Transform();
			child->translate(glm::vec3(0.f, 1.25f, -1.f));
			child->scale(0.25f);
			goal->addChild(child);
			c = new ComponentRigidBodyBox(glm::vec3(2.f, 1.25f, colThicc), true);
			child->addComponent(c);
		}

		ComponentGoal * c = new ComponentGoal(ball, glm::vec3(2, 2.5f, -1), glm::vec3(-2, 0, 0));
		goal->addComponent(c);

		scene1->addTransform(goal);
	}
	//Goalpost - BLUE
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_BLUE));
		Transform * goal = new Transform(model_goal, mat);

		goal->translate(glm::vec3(0, 0, -7.5) + groundPosition);

		{
			float colThicc = 0.05f;

			Transform * child = new Transform();
			child->translate(glm::vec3(-2.f, 1.25f, -0.5f));
			child->scale(0.25f);
			goal->addChild(child);
			ComponentRigidBodyBox * c = new ComponentRigidBodyBox(glm::vec3(colThicc, 1.25f, 0.5f), true);
			child->addComponent(c);


			child = new Transform();
			child->translate(glm::vec3(2.f, 1.25f, -0.5f));
			child->scale(0.25f);
			goal->addChild(child);
			c = new ComponentRigidBodyBox(glm::vec3(colThicc, 1.25f, 0.5f), true);
			child->addComponent(c);

			child = new Transform();
			child->translate(glm::vec3(0.f, 2.5f, -0.5f));
			child->scale(0.25f);
			goal->addChild(child);
			c = new ComponentRigidBodyBox(glm::vec3(2.f, colThicc, 0.5f), true);
			child->addComponent(c);

			child = new Transform();
			child->translate(glm::vec3(0.f, 1.25f, -1.f));
			child->scale(0.25f);
			goal->addChild(child);
			c = new ComponentRigidBodyBox(glm::vec3(2.f, 1.25f, colThicc), true);
			child->addComponent(c);
		}

		ComponentGoal * c = new ComponentGoal(ball, glm::vec3(2, 2.5f, -1), glm::vec3(-2, 0, 0));
		goal->addComponent(c);

		scene1->addTransform(goal);
	}

	scene1->LateInit();
}

void ProjectManager::initAudio() {
	//sound_BG = new Sounds("Sound/67032__robinhood76__00892-funeral-silencium-trumpet.wav");
	

	//sound_BG->Play();
	sound_Hit = new Sounds("Sound/fallsmall.wav");
}

void ProjectManager::initProject() {
	//TODO
}

void ProjectManager::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Draw SceneGraph objects
	sceneGlobal->draw(headPose, projection);
	sceneMenu->draw(headPose, projection);
	scene1->draw(headPose, projection);

	/*
	switch (curScene) {
	case(SCENE_MENU):
		sceneMenu->draw(headPose, projection);
		break;
	case(SCENE_1):
		scene1->draw(headPose, projection);
		break;
	default:
		curScene = SCENE_MENU;
		break;
	}
	*/

	//Debug Draw
	lines->draw(headPose, projection, glm::mat4(1));
	physics->draw(headPose, projection);
}

void ProjectManager::update(double deltaTime) {
	physics->update(deltaTime);
	sceneGlobal->update(deltaTime);
	sceneMenu->update(deltaTime);
	scene1->update(deltaTime);

	//TODO - handle scene changes (maybe use enum with a switch statement)
	/*
	switch (curScene) {
	case(SCENE_MENU):
		sceneMenu->update(deltaTime);
		break;
	case(SCENE_1):
		scene1->update(deltaTime);
		break;
	default:
		curScene = SCENE_MENU;
		break;
	}
	*/

	if (startedNetwork) {
		sendPlayerData();
		receivePackets();
	}
	else
		networkingSetup();

	testing();
}

void ProjectManager::updateHands(glm::mat4 left, glm::mat4 right) {
	{
		handLModel->setToWorld(left);
		handLModel->scale(0.25f);
		handRModel->setToWorld(right);
		handRModel->scale(0.25f);
	}

	handL->setToWorld(left);
	handL->scale(glm::vec3(0.015f));
	handR->setToWorld(right);
	handR->scale(glm::vec3(0.015f));

	physics->newRColPos(handR->getChild(1)->getPosition(false), glm::quat_cast(player->getCompleteToWorld() * right), stickR->getlinVelo());
	physics->newLColPos(handL->getChild(1)->getPosition(false), glm::quat_cast(player->getCompleteToWorld() * left), stickL->getlinVelo());
}

void ProjectManager::updateHead(glm::mat4 hmd) {
	head->setToWorld(hmd);
	head->scale(0.1f);
}

void ProjectManager::updateLightCameraPos(glm::mat4 eye) {
	Light::setCameraPos(eye[3]);
}

glm::vec3 ProjectManager::getPlayerPosition() {
	return player->getPosition();
}

glm::quat ProjectManager::getPlayerRotation() {
	return player->getRotation();
}

glm::mat4 ProjectManager::getPlayerToWorld() {
	return player->getCompleteToWorld();
}

glm::vec3 ProjectManager::getLeftHandPosition() {
	return handL->getPosition(false);
}

glm::vec3 ProjectManager::getRightHandPosition() {
	return handR->getPosition(false);
}

glm::vec3 ProjectManager::getStickVelocity(bool left) {
	if (left)
		return stickL->getlinVelo();
	else
		return stickR->getlinVelo();

}

Sounds* ProjectManager::getSoundEffect(SoundEffect soundEffect) {
	switch (soundEffect) {
	case HIT_SOUND:
		return sound_Hit;
		break;
	default:
		break;
	}
}

void ProjectManager::testing() {

}

void ProjectManager::networkingSetup() {
	//Has networking setup begun?
	if (startedNetwork) return;

	//If not, is user attempting to begin?
	if (Input::getButtonStickL()) {	//TODO - change how to host/join server
		//Start Server
		startedNetwork = true;
		serverConnect();
	}
	else if (Input::getButtonStickR()) {
		//Connect to server
		startedNetwork = true;
		clientConnect();
	}
}

void ProjectManager::serverConnect() {
	Server::startServer();
	clientConnect();
	client->player = 1;

	handL->material->color = glm::vec3(COLOR_RED);
	handR->material->color = glm::vec3(COLOR_RED);
	otherHandL->material->color = glm::vec3(COLOR_BLUE);
	otherHandR->material->color = glm::vec3(COLOR_BLUE);
	otherHead->material->color = glm::vec3(COLOR_BLUE);
}

void ProjectManager::clientConnect() {
	client->joinServer();
	client->player = 2;
	handL->material->color = glm::vec3(COLOR_BLUE);
	handR->material->color = glm::vec3(COLOR_BLUE);
	otherHandL->material->color = glm::vec3(COLOR_RED);
	otherHandR->material->color = glm::vec3(COLOR_RED);
	otherHead->material->color = glm::vec3(COLOR_RED);
}

void ProjectManager::stopNetworking() {
	startedNetwork = false;

}

void ProjectManager::sendPlayerData() {
	client->sendPlayerDataPacket(head->getCompleteToWorld(), HEAD);
	client->sendPlayerDataPacket(handL->getCompleteToWorld(), HAND_LEFT);
	client->sendPlayerDataPacket(handR->getCompleteToWorld(), HAND_RIGHT);
}

void ProjectManager::receivePackets() {
	//Init and Exit Packets
	{
		if (client->getInitPacket()) {	//Init Packet
			otherHandL->isActive = true;
			otherHandR->isActive = true;
			otherHead->isActive = true;
		}
		else {							//Exit Packet
			otherHandL->isActive = false;
			otherHandR->isActive = false;
			otherHead->isActive = false;
		}
	}

	//Player Packets
	{
		std::vector<Packet> packets = client->getPlayerPackets();

		for (int i = 0; i < packets.size(); i++) {
			Packet packet = packets[i];
			switch (packet.dataType) {
			case HEAD:
				otherHead->setToWorld(packet.toWorld);
				break;
			case HAND_LEFT:
				otherHandL->setToWorld(packet.toWorld);
				break;
			case HAND_RIGHT:
				otherHandR->setToWorld(packet.toWorld);
				break;
			default:
				std::cerr << "got trash" << std::endl;
				break;
			}
		}
	}
}





