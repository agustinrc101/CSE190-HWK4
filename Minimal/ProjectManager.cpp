#define STB_IMAGE_IMPLEMENTATION

#include "ProjectManager.h"
#include "Definitions.h"
//Shaders and textures
#include "Shaders.h"
#include "Shader.h"
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
#include "ComponentTest1.h"
#include "ComponentTest2.h"
#include "ComponentTest3.h"
#include "ComponentTest4.h"

//Init Shaders
GLint Shaders::colorShader = 0;
GLint Shaders::textureShader = 0;
GLint Shaders::skyboxShader = 0;
//Init Textures
GLuint Textures::textureSteam = 0;
GLuint Textures::textureSkybox = 0;
GLuint Textures::textureGrip1Albedo = 0;
GLuint Textures::textureGrip2Albedo = 0;

//Init SceneGraph
SceneGraph * sceneGraph;
//Declare Models
Model * model_sphere;
Model * model_cube;
//Declare Objects
Transform * handL; 
Transform * handR;
//Declare Skyboxes
Skybox * skybox;

//Debug
Lines * lines;

ProjectManager::~ProjectManager() {
	//Delete SceneGraph	(also deletes transforms, models, and components) --> make sure to use every declared model/component/transform
	delete(sceneGraph);
	//Delete nontransform objects
	delete(skybox);
	//Delete lines
	delete(lines);
	//Delete shaders
	Shaders::deleteShaders();
	//Delete textures
	Textures::deleteTextures();
}

ProjectManager::ProjectManager() {
	initShaders();
	initTextures();
	initModels();
	initObjects();
	initSceneGraph();
	initAudio();
	initProject();
	initNetworking();
}

void ProjectManager::initShaders() {
	Shaders::setColorShader(LoadShaders(SHADER_COLOR_VERTEX, SHADER_COLOR_FRAGMENT));
	Shaders::setTextureShader(LoadShaders(SHADER_TEXTURE_VERTEX, SHADER_TEXTURE_FRAGMENT));
	Shaders::setSkyboxShader(LoadShaders(SHADER_SKYBOX_VERTEX, SHADER_SKYBOX_FRAGMENT));
}

void ProjectManager::initTextures(){
	Textures::setTextureSkybox(LoadCubeMap(TEXTURE_SKYBOX_LEFT));
	Textures::setTextureSteam(LoadTextures(TEXTURE_CUBE_STEAM));
	Textures::setTextureGrip1Albedo(LoadTextures(TEXTURE_GRIP1_ALBEDO));
	Textures::setTextureGrip2Albedo(LoadTextures(TEXTURE_GRIP2_ALBEDO));
}

void ProjectManager::initModels() {
	model_sphere = new Model(MODEL_SPHERE);
	model_cube = new Model(MODEL_CUBE);
}

void ProjectManager::initObjects() {
	//Initialize non-scenegraph objects here
	skybox = new Skybox(Textures::getTextureSkybox());
	lines = new Lines(new Material(Shaders::getColorShader(), glm::vec3(COLOR_GREEN)));
}

void ProjectManager::initSceneGraph() {
	sceneGraph = new SceneGraph();

	//Right hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
		handL = new Transform(model_sphere, mat);

		ComponentTest1 * test1 = new ComponentTest1();
		ComponentTest3 * test3 = new ComponentTest3(false);

		handL->addComponent(test1);
		handL->addComponent(test3);

		sceneGraph->addTransform(handL);
	}
	//Left Hand setup
	{
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_RED));
		handR = new Transform(model_sphere, mat);

		ComponentTest3 * test3 = new ComponentTest3(true);
		handR->addComponent(test3);

		sceneGraph->addTransform(handR);
	}
	//
	//Initialize scenegraph objects here
	//
	//[Transform samples]
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureSteam());
		Transform * transform = new Transform(model_sphere, mat);
		
		transform->scale(0.1f);
		transform->translate(glm::vec3(0, 0, -2));

		ComponentTest2 * c = new ComponentTest2(AXIS_X_POSITIVE + AXIS_Z_NEGATIVE);
		transform->addComponent(c);

		sceneGraph->addTransform(transform);
		lines->addVertex(transform->getPosition());
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_BLUE), Textures::getTextureGrip1Albedo());
		Transform * transform = new Transform(model_cube, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(3, 0, -1.5));

		ComponentTest2 * c = new ComponentTest2(AXIS_Y_POSITIVE);
		transform->addComponent(c);

		sceneGraph->addTransform(transform);
		lines->addVertex(transform->getPosition());
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrip2Albedo());
		Transform * transform = new Transform(model_cube, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(-3, 0, -1.5));

		ComponentTest2 * c = new ComponentTest2(AXIS_Y_NEGATIVE);
		transform->addComponent(c);

		sceneGraph->addTransform(transform);
		lines->addVertex(transform->getPosition());
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_GREEN), Textures::getTextureSteam());
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(0, 3, -2));

		ComponentTest2 * c = new ComponentTest2(AXIS_X_NEGATIVE + AXIS_Z_POSITIVE);
		transform->addComponent(c);

		sceneGraph->addTransform(transform);
		lines->addVertex(transform->getPosition());
	}
	{
		Material * mat = new Material(Shaders::getTextureShader(), glm::vec3(COLOR_WHITE), Textures::getTextureGrip1Albedo());
		Transform * transform = new Transform(model_sphere, mat);

		transform->scale(0.1f);
		transform->translate(glm::vec3(3, 6, -2));

		ComponentTest4 * c4 = new ComponentTest4(AXIS_X_NEGATIVE, 3.0f, 0.2f);
		transform->addComponent(c4);
		ComponentTest2 * c2 = new ComponentTest2(AXIS_Y_NEGATIVE, 3.0f);
		transform->addComponent(c2);

		sceneGraph->addTransform(transform);
		lines->addVertex(transform->getPosition());
	}
}

void ProjectManager::initAudio() {
	//TODO
}

void ProjectManager::initProject() {
	//TODO
}

void ProjectManager::initNetworking() {
	//TODO
}

void ProjectManager::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Draw skybox skybox
	skybox->draw(headPose, projection, Shaders::getSkyboxShader());
	//Draw SceneGraph objects
	sceneGraph->draw(headPose, projection);
	//Debug Draw
	lines->draw(headPose, projection, glm::mat4(1));
}

void ProjectManager::update(double deltaTime) {
	//TODO - handle networking packets
	//TODO - handle audio
	sceneGraph->update(deltaTime);
}

void ProjectManager::updateHands(glm::mat4 left, glm::mat4 right) {
	handL->setToWorld(left);
	handL->scale(0.015f);
	handR->setToWorld(right);
	handR->scale(0.015f);
}