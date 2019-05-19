#include "Quad.h"

Quad::Quad(float size){
	initPlane(size);
	initBuffers();
}


Quad::~Quad(){
	indices.clear();
	vertices.clear();
	texCoords.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Quad::draw(glm::mat4 projection, glm::mat4 headPose, glm::mat4 M, Material * mat) {
	glm::mat4 m = M * toWorld;

	glUseProgram(mat->shader);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "view"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "model"), 1, GL_FALSE, &m[0][0]);
	glUniform3f(glGetUniformLocation(mat->shader, "rgb"), mat->color.r, mat->color.g, mat->color.b);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mat->TEX);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Quad::update() { }

void Quad::initPlane(float size) {
	float half = size / 2.0f;

	//Vertices
	vertices.push_back(glm::vec3(-half, -half, 0));	//left, down
	vertices.push_back(glm::vec3(half, -half, 0));	//right, down
	vertices.push_back(glm::vec3(half, half, 0));	//right, top
	vertices.push_back(glm::vec3(-half, half, 0));	//left, top

	//indices
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	//Tex Coords
	texCoords.push_back(glm::vec2(0, 0)); texCoords.push_back(glm::vec2(1, 0));
	texCoords.push_back(glm::vec2(1, 1)); texCoords.push_back(glm::vec2(0, 1));
}

void Quad::initBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);

	//passes vertices
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//passes indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// passes texture coords
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &(texCoords[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}