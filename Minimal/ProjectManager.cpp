#define STB_IMAGE_IMPLEMENTATION

#include "ProjectManager.h"
#include "Definitions.h"
//Shaders and textures
#include "Shaders.h"
#include "Shader.h"
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
//Sound
#include "Sounds.h"
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
//Init Textures
GLuint Textures::textureSteam = 0;
GLuint Textures::textureSkybox = 0;
GLuint Textures::textureGrip1Albedo = 0;
GLuint Textures::textureGrip2Albedo = 0;
GLuint Textures::textureStick = 0;
GLuint Textures::textureGrass = 0;
//Init Networking
Client * Client::client = 0;
Client * client;
bool startedNetwork = false;
bool Server::serverOn = false;
//Init Physics
Physics * Physics::physics = 0;

//Init Sound
Sounds * sound;
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
//Declare Important Objects
Transform * player;
Transform * head;
Transform * handL; 
Transform * handR;
Transform * otherHead;
Transform * otherHandL;
Transform * otherHandR;


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
	delete(sound);
	delete(physics);
}

ProjectManager::ProjectManager() {
	initBulletPhysics();
	initShadersAndLighting();
	initTextures();
	initModels();
	initObjects();
	initSceneGraphs();
	initAudio();
	initProject();
	client = new Client();
}

void ProjectManager::initBulletPhysics() {
	physics = new Physics();
}

void ProjectManager::initShadersAndLighting() {
	Light::setLightPosition(glm::vec3(-10, 10, 5));
	Light::setLightColor(glm::vec3(COLOR_WHITE));

	Shaders::setColorShader(LoadShaders(SHADER_COLOR_VERTEX, SHADER_COLOR_FRAGMENT));
	Shaders::setTextureShader(LoadShaders(SHADER_TEXTURE_VERTEX, SHADER_TEXTURE_FRAGMENT));
	Shaders::setSkyboxShader(LoadShaders(SHADER_SKYBOX_VERTEX, SHADER_SKYBOX_FRAGMENT));
}

void ProjectManager::initTextures(){
	Textures::setTextureSkybox(LoadCubeMap(TEXTURE_SKYBOX_LEFT));
	Textures::setTextureSteam(LoadTextures(TEXTURE_CUBE_STEAM));
	Textures::setTextureGrip1Albedo(LoadTextures(TEXTURE_GRIP1_ALBEDO));
	Textures::setTextureGrip2Albedo(LoadTextures(TEXTURE_GRIP2_ALBEDO));
	Textures::setTextureStick(LoadTextures(TEXTURE_STICK));
	Textures::setTextureGrass(LoadTextures(TEXTURE_GRASS));
}

void ProjectManager::initModels() {
	model_sphere = new Model(MODEL_SPHERE);
	model_cube = new Model(MODEL_CUBE);
	model_plane = new Model(MODEL_PLANE);
	model_stick = new Model(MODEL_STICK);
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

		ComponentRigidBodyStick * col = new ComponentRigidBodyStick(0.10f);
		handR->addComponent(col);

		mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureStick());
		Transform * stick = new Transform(model_stick, mat);

		stick->scale(glm::vec3(0.5f, 0.7f, 0.5f));
		stick->translate(glm::vec3(0, -175, 0));

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

		mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureStick());
		Transform * stick = new Transform(model_stick, mat);

		stick->scale(glm::vec3(0.5f, 0.7f, 0.5f));
		stick->translate(glm::vec3(0, -175, 0));

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
	//Player parent
	{
		Material * mat = new Material();
		player = new Transform();
		player->name = "Player";

		player->addChild(head);
		player->addChild(handL);
		player->addChild(handR);

		ComponentPogoMovement * c1 = new ComponentPogoMovement(head, handR, colliderR);
		player->addComponent(c1);
		c1 = new ComponentPogoMovement(head, handL, colliderL);
		player->addComponent(c1);
		
		sceneGlobal->addTransform(player);
	}

	//Other player setup
	//Right hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_GREEN));
		otherHandR = new Transform(model_sphere, mat);
		otherHandR->name = "Other Player's Right Hand";
		sceneGlobal->addTransform(otherHandR);

	}
	//Left Hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_ORANGE));
		otherHandL = new Transform(model_sphere, mat);
		otherHandL->name = "Other Player's Left Hand";

		sceneGlobal->addTransform(otherHandL);
	}
	//Head setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_YELLOW));
		otherHead = new Transform(model_cube, mat);
		otherHead->name = "Other Player's Head";

		sceneGlobal->addTransform(otherHead);
	}
}

void ProjectManager::initMenuScene() {
	sceneMenu = new SceneGraph(new Skybox(), 0);	//No parameters on Skybox and a 0 don't render a skybox

	//==================================
	//Initialize sceneMenu objects here
	//==================================
	{

	}
}

void ProjectManager::initScene1() {
	scene1 = new SceneGraph(new Skybox(Textures::getTextureSkybox()), Shaders::getSkyboxShader());
	//==================================
	//Initialize scene1 objects here
	//==================================
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrass());
		Transform * transform = new Transform(model_plane, mat);

		transform->translate(glm::vec3(0, -1.7f, 0));	//ACTUAL GROUND POS
		//transform->translate(glm::vec3(0, -.4f, 0));	//DEBUG GROUND POS
		
		ComponentRigidBodyPlane * col = new ComponentRigidBodyPlane(100);
		transform->addComponent(col);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_RED), Textures::getTextureSteam());
		Transform * transform = new Transform(model_sphere, mat, false);

		//transform->scale(0.1f);
		transform->translate(glm::vec3(0, 0, -10));

		ComponentRotate * c = new ComponentRotate(AXIS_X_POSITIVE + AXIS_Z_NEGATIVE);
		transform->addComponent(c);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_BLUE), Textures::getTextureGrip1Albedo());
		Transform * transform = new Transform(model_cube, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(3, 0, -5));

		ComponentRotate * c = new ComponentRotate(AXIS_Y_POSITIVE);
		transform->addComponent(c);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrip2Albedo());
		Transform * transform = new Transform(model_cube, mat, false);

		transform->scale(0.1f);
		transform->translate(glm::vec3(-1, 1, 0));

		ComponentRigidBodyBox * col = new ComponentRigidBodyBox(glm::vec3(0.2f));
		transform->addComponent(col);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_CYAN), Textures::getTextureSteam());
		Transform * transform = new Transform(model_sphere, mat, false);

		transform->scale(0.3f);
		transform->translate(glm::vec3(-2, 100, -0.8f));

		ComponentRigidBodySphere * col = new ComponentRigidBodySphere(0.3f);
		transform->addComponent(col);

		scene1->addTransform(transform);
	}
}

void ProjectManager::initAudio() {
	sound = new Sounds();
}

void ProjectManager::initProject() {
	//TODO
}

void ProjectManager::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Draw SceneGraph objects
	sceneGlobal->draw(headPose, projection);
	sceneMenu->draw(headPose, projection);
	scene1->draw(headPose, projection);
	//Debug Draw
	lines->draw(headPose, projection, glm::mat4(1));
	physics->draw(headPose, projection);
}

void ProjectManager::update(double deltaTime) {
	physics->update(deltaTime);
	
	//TODO - handle scene changes (maybe use enum with a switch statement)
	
	sceneGlobal->update(deltaTime);
	sceneMenu->update(deltaTime);
	scene1->update(deltaTime);
	
	if (startedNetwork) {
		sendPlayerData();
		receivePackets();
	}
	else 
		networkingSetup();

	testing();
}

void ProjectManager::updateHands(glm::mat4 left, glm::mat4 right) {
	handL->setToWorld(left);
	handL->scale(0.015f);
	handR->setToWorld(right);
	handR->scale(0.015f);

	physics->newRColPos(handR->getCol()->getPosition(false));
}

void ProjectManager::updateHead(glm::mat4 hmd) {
	head->setToWorld(hmd);
	head->scale(0.1f);
}

void ProjectManager::updateLightCameraPos(glm::mat4 eye){
	Light::setCameraPos(eye[3]);
}

glm::vec3 ProjectManager::getPlayerPosition() {
	return player->getPosition();
}

glm::quat ProjectManager::getPlayerRotation() {
	return player->getRotation();
}

void ProjectManager::testing() {
	//Testing code here
	if (Input::getButtonStickL()) {
		print(handL->getToWorld());
		print(handL->getCompleteToWorld());
	}

}

void ProjectManager::networkingSetup() {
	//Has networking setup begun?
	if (startedNetwork) return;	

	//If not, is user attempting to begin?
	if (Input::getButtonA()) {	//TODO - change how to host/join server
		//Start Server
		startedNetwork = true;
		serverConnect();
	}
	else if (Input::getButtonX()) {
		//Connect to server
		startedNetwork = true;
		clientConnect();
	}
}

void ProjectManager::serverConnect() {
	Server::startServer();
	clientConnect();
}

void ProjectManager::clientConnect() {
	client->joinServer();
}

void ProjectManager::stopNetworking(){
	startedNetwork = false;

}

void ProjectManager::sendPlayerData() {
		client->sendPlayerDataPacket(head->getCompleteToWorld(), HEAD);
		client->sendPlayerDataPacket(handL->getCompleteToWorld(), HAND_LEFT);
		client->sendPlayerDataPacket(handR->getCompleteToWorld(), HAND_RIGHT);
}

void ProjectManager::receivePackets() {
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