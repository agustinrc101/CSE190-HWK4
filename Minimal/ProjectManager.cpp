#include "ProjectManager.h"
#include "Definitions.h"
#include "Shaders.h"
#include "Textures.h"
#include "Shader.h"
#include "Transform.h"
#include "TexturedCube.h"
#include "Skybox.h"

//Components
#include "ComponentTest1.h"
#include "ComponentTest2.h"
#include "ComponentTest3.h"

//Init Shaders
GLint Shaders::colorShader = 0;
GLint Shaders::textureShader = 0;
GLint Shaders::skyboxShader = 0;
GLint Shaders::renderedTextureShader = 0;
GLint Shaders::LCDisplayShader = 0;
//Init Textures
GLuint Textures::textureSteam = 0;

//Declare Models
Model * sphere;
//Declare Objects
Transform * handL; 
Transform * handR;
//Declare Skyboxes
Skybox * skyboxCustom;

//TESTING
ComponentTest1 * test1;
ComponentTest2 * test2;
ComponentTest3 * test3;

ProjectManager::~ProjectManager() {
	//Delete objects (also deletes models)
	delete(handL);
	delete(handR);
	delete(skyboxCustom);
	//Delete shaders
	Shaders::deleteShaders();
}

ProjectManager::ProjectManager() {
	initShaders();
	initTextures();
	initModels();
	initObjects();
	initValues();
}

void ProjectManager::initShaders() {

	Shaders::setColorShader(LoadShaders(SHADER_COLOR_VERTEX, SHADER_COLOR_FRAGMENT));
	Shaders::setTextureShader(LoadShaders(SHADER_TEXTURE_VERTEX, SHADER_TEXTURE_FRAGMENT));
	Shaders::setSkyboxShader(LoadShaders(SHADER_SKYBOX_VERTEX, SHADER_SKYBOX_FRAGMENT));
	Shaders::setRenderedTextureShader(LoadShaders(SHADER_RENDERED_TEXTURE_VERTEX, SHADER_RENDERED_TEXTURE_FRAGMENT));
	Shaders::setLCDisplayShader(LoadShaders(SHADER_LCDISPLAY_VERTEX, SHADER_LCDISPLAY_FRAGMENT));
}

void ProjectManager::initTextures(){
	Textures::setTextureSteam(LoadTextures(TEXTURE_CUBE_STEAM));
}

void ProjectManager::initModels() {
	sphere = new Model(MODEL_SPHERE);
}

void ProjectManager::initObjects() {
	skyboxCustom = new Skybox(TEXTURE_SKYBOX_LEFT);
	handL = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_CYAN));
	handR = new Transform(sphere, Shaders::getColorShader(), glm::vec3(COLOR_RED));
}

void ProjectManager::initValues() {
	test1 = new ComponentTest1();
	test2 = new ComponentTest2();
	test3 = new ComponentTest3();

	handL->addComponent(test1);
	handL->addComponent(test2);
	handR->addComponent(test3);
}

void ProjectManager::draw(glm::mat4 headPose, glm::mat4 projection, int eye) {
	//Draw skybox skybox
	skyboxCustom->draw(projection, headPose, Shaders::getSkyboxShader());
	//Draw hands
	handL->draw(headPose, projection);
	handR->draw(headPose, projection);
}

void ProjectManager::update(double deltaTime) {
	handL->update(deltaTime);
	handR->update(deltaTime);
}

void ProjectManager::updateHands(glm::mat4 left, glm::mat4 right) {
	handL->setToWorld(left);
	handL->scale(0.015f);
	handR->setToWorld(right);
	handR->scale(0.015f);
}