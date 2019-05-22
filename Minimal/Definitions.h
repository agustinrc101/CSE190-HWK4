#ifndef H_DEFINITIONS
#define H_DEFINITIONS
#pragma once

#include <iostream>

//Shaders
#define SHADER_COLOR_VERTEX "./shaders/color.vert"
#define SHADER_COLOR_FRAGMENT "./shaders/color.frag"
#define SHADER_TEXTURE_VERTEX "./shaders/TextureShader.vert"
#define SHADER_TEXTURE_FRAGMENT "./shaders/TextureShader.frag"
#define SHADER_SKYBOX_VERTEX "./shaders/skybox.vert"
#define SHADER_SKYBOX_FRAGMENT "./shaders/skybox.frag"

//Textures
#define TEXTURE_SKYBOX_LEFT "skybox/left"
#define TEXTURE_SKYBOX_RIGHT "skybox/right"
#define TEXTURE_CUBE_STEAM "textures/steam/albedo.ppm"
#define TEXTURE_GRIP1_ALBEDO "textures/Grip1/Grip1_Albedo.png"
#define TEXTURE_GRIP2_ALBEDO "textures/Grip2/Grip2_Albedo.png"
#define TEXTURE_TRASH "textures/trash.png"

//Models
#define MODEL_SPHERE "models/sphere.obj"
#define MODEL_CUBE "models/cube.obj"
#define MODEL_PLANE "models/plane.obj"

//Colors
#define COLOR_RED 1, 0, 0
#define COLOR_GREEN 0, 1, 0
#define COLOR_BLUE 0, 0, 1
#define COLOR_PURPLE 0.5f, 0, 1
#define COLOR_YELLOW 1, 1, 0
#define COLOR_ORANGE 1, 0.56f, 0
#define COLOR_CYAN 0, 1, 1
#define COLOR_BLACK 0, 0, 0
#define COLOR_WHITE 1, 1, 1

//Axes
#define AXIS_X_POSITIVE glm::vec3(1, 0, 0)
#define AXIS_X_NEGATIVE glm::vec3(-1, 0, 0)
#define AXIS_Y_POSITIVE glm::vec3(0, 1, 0)
#define AXIS_Y_NEGATIVE glm::vec3(0, -1, 0)
#define AXIS_Z_POSITIVE glm::vec3(0, 0, -1)
#define AXIS_Z_NEGATIVE glm::vec3(0, 0, 1)

//Variables
#define MATH_PI 3.1415926535897932384626433832795f

//Enums


//========
//PRINTING
//========
//Print Matrices
static void print(glm::mat4 c){
	std::cout << "============MATRIX============" << std::endl;
	std::cout << c[0][0] << ", " << c[1][0] << ", " << c[2][0] << ", " << c[3][0] << std::endl;
	std::cout << c[0][1] << ", " << c[1][1] << ", " << c[2][1] << ", " << c[3][1] << std::endl;
	std::cout << c[0][2] << ", " << c[1][2] << ", " << c[2][2] << ", " << c[3][2] << std::endl;
	std::cout << c[0][3] << ", " << c[1][3] << ", " << c[2][3] << ", " << c[3][3] << std::endl;
	std::cout << "==============================" << std::endl;
};
//Print Vec4
static void print(glm::vec4 v){
	std::cout << "Vector4: " << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
};
//Print Vec3
static void print(glm::vec3 v) {
	std::cout << "Vector3: " << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
};
//Print Vec2
static void print(glm::vec2 v) {
	std::cout << "Vector2: " << v[0] << ", " << v[1] << std::endl;
}

//========
//Helpers
//========
//Calculate triangle normals
static glm::vec3 calculateNormal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2){
	//Vertex variables
	glm::vec3 pa = p1;
	glm::vec3 pb = p2;
	glm::vec3 pc = p0;

	//Init the direction variables
	glm::vec3 vr, vu, vn;

	//Calculate right, up, and normal vectors
	vr = pb - pa;
	vr = glm::normalize(vr);	//right

	vu = pc - pa;
	vu = glm::normalize(vu);	//up

	vn = glm::cross(vr, vu);
	return glm::normalize(vn);	//normal
}
#endif