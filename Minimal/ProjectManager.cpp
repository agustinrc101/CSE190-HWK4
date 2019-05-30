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
GLuint Textures::textureTrash = 0;
//Init Networking
Client * Client::client = 0;
Client * client;
bool startedNetwork = false;
bool Server::serverOn = false;

//Init SceneGraph
SceneGraph * sceneGlobal;
SceneGraph * sceneMenu;
SceneGraph * scene1;
//Declare Models
Model * model_sphere;
Model * model_cube;
Model * model_plane;
//Declare Important Objects
Transform * handL; 
Transform * handR;

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
}

ProjectManager::ProjectManager() {
	initShadersAndLighting();
	initTextures();
	initModels();
	initObjects();
	initSceneGraphs();
	initAudio();
	initProject();
	client = new Client();
}

void ProjectManager::initShadersAndLighting() {
	Light::setLightPosition(glm::vec3(-100, 100, 25));
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
	Textures::setTextureTrash(LoadTextures(TEXTURE_TRASH));
}

void ProjectManager::initModels() {
	model_sphere = new Model(MODEL_SPHERE);
	model_cube = new Model(MODEL_CUBE);
	model_plane = new Model(MODEL_PLANE);
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
	sceneGlobal = new SceneGraph(new Skybox(), 0);	//This does not render a skybox

	//==================================
	//Initialize sceneGlobal objects here
	//==================================
	//Right hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
		handL = new Transform(model_sphere, mat);
		handL->name = "Right Hand";

		ComponentPrintPosition * c1 = new ComponentPrintPosition();
		handL->addComponent(c1);
		ComponentSendTestPacket * c2 = new ComponentSendTestPacket();
		handL->addComponent(c2);

		sceneGlobal->addTransform(handL);
	}
	//Left Hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_RED));
		handR = new Transform(model_sphere, mat);
		handR->name = "Left Hand";

		sceneGlobal->addTransform(handR);
	}
	//Head setup
	//TODO

	//Other player setup
	//TODO
}

void ProjectManager::initMenuScene() {
	sceneMenu = new SceneGraph(new Skybox(), 0);	//This does not render a skybox

	//==================================
	//Initialize scene1 objects here
	//==================================
}

void ProjectManager::initScene1() {
	scene1 = new SceneGraph(new Skybox(Textures::getTextureSkybox()), Shaders::getSkyboxShader());
	//scene1 = new SceneGraph(new Skybox(), 0);
	
	//==================================
	//Initialize scene1 objects here
	//==================================
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureSteam());
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(0, 0, -2));

		ComponentRotate * c = new ComponentRotate(AXIS_X_POSITIVE + AXIS_Z_NEGATIVE);
		transform->addComponent(c);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_BLUE), Textures::getTextureGrip1Albedo());
		Transform * transform = new Transform(model_cube, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(3, 0, -1.5));

		ComponentRotate * c = new ComponentRotate(AXIS_Y_POSITIVE);
		transform->addComponent(c);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrip2Albedo());
		Transform * transform = new Transform(model_cube, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(-3, 0, -1.5));

		ComponentRotate * c = new ComponentRotate(AXIS_Y_NEGATIVE);
		transform->addComponent(c);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_ORANGE), Textures::getTextureSteam());
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(0, 3, -2));

		ComponentRotate * c = new ComponentRotate(AXIS_X_NEGATIVE + AXIS_Z_POSITIVE);
		transform->addComponent(c);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrip1Albedo());
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(3, 6, -2));

		ComponentMoveTest * c4 = new ComponentMoveTest(AXIS_X_NEGATIVE, 3.0f, 0.2f);
		transform->addComponent(c4);
		ComponentRotate * c2 = new ComponentRotate(AXIS_Y_NEGATIVE, 3.0f);
		transform->addComponent(c2);

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrip1Albedo());
		Transform * transform = new Transform(model_plane, mat);

		transform->scale(1.0f);
		transform->translate(glm::vec3(0, -2, 0));

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureTrash());
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.2f);
		transform->translate(glm::vec3(-5, 0, 0));

		ComponentRotate * c2 = new ComponentRotate(AXIS_Y_POSITIVE, 2.0f);
		transform->addComponent(c2);

		//Child
		{
			mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_YELLOW));
			Transform * child = new Transform(model_cube, mat);

			child->scale(0.5f);
			child->translate(glm::vec3(0, 4.5f, 0));

			ComponentMoveTest * c4 = new ComponentMoveTest(AXIS_X_POSITIVE);
			child->addComponent(c4);

			transform->addChild(child);
		}

		scene1->addTransform(transform);
	}
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_WHITE));
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.2f);
		
		scene1->addTransform(transform);
	}

}

void ProjectManager::initAudio() {
	//TODO
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
}

bool a_press = false;
void ProjectManager::update(double deltaTime) {
	networkingSetup();
	//TODO - handle networking packets
	//TODO - handle audio
	//TODO - handle scene changes (maybe use enum with a switch statement)
	sceneGlobal->update(deltaTime);
	sceneMenu->update(deltaTime);
	scene1->update(deltaTime);

	testing();
}

void ProjectManager::updateHands(glm::mat4 left, glm::mat4 right) {
	handL->setToWorld(left);
	handL->scale(0.015f);
	handR->setToWorld(right);
	handR->scale(0.015f);
}

void ProjectManager::updateHead(glm::mat4 eye){
	Light::setCameraPos(eye[3]);
}

void ProjectManager::testing() {
	//Testing code here


}

void ProjectManager::networkingSetup() {
	//Has networking setup begun?
	if (startedNetwork) return;	
	//If not, is user attempting to begin?
	if (!(Input::getButtonA() || Input::getButtonX())) return;	//TODO - change how to host/join server
	//If so, initialize networking
	startedNetwork = true;
	initNetworking();
}


void ProjectManager::initNetworking() {
	std::cout << "init networking" << std::endl;
	Server::startServer();
	client->joinServer();
}

void ProjectManager::stopNetworking(){
	std::cout << "stop networking" << std::endl;
	startedNetwork = false;

}