#include "TexturedCube.h"

TexturedCube::TexturedCube(Material * mat){
	this->toWorld = glm::mat4(1.0f);
	initCube(1);
	initBuffers();

	material = mat;
}

TexturedCube::~TexturedCube(){
	indices.clear();
	vertices.clear();
	texCoords.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void TexturedCube::setPosition(glm::vec3 pos) {
	toWorld[3] = glm::vec4(pos.x, pos.y, pos.z, 1);
}

void TexturedCube::setScale(float scale) {
	glm::vec4 pos = toWorld[3];
	toWorld = glm::mat4(1.0f);
	toWorld[3] = pos;
	toWorld = glm::scale(toWorld, glm::vec3(scale, scale, scale));
}

void TexturedCube::draw(glm::mat4 projection, glm::mat4 headPose,glm::mat4 M, Material * mat){
	glm::mat4 m = M * toWorld;

	glUseProgram(mat->shader);

	glUniform1i(glGetUniformLocation(mat->shader, "TexCoords"), 0);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "Projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "View"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "Model"), 1, GL_FALSE, &m[0][0]);
	glUniform3f(glGetUniformLocation(mat->shader, "Color"), mat->color.r, mat->color.g, mat->color.b);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mat->TEX);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	

	glBindVertexArray(0);
}

void TexturedCube::draw(glm::mat4 projection, glm::mat4 headPose, glm::mat4 M) {
	glm::mat4 m = M * toWorld;

	glUseProgram(material->shader);

	glUniform1i(glGetUniformLocation(material->shader, "TexCoords"), 0);
	glUniformMatrix4fv(glGetUniformLocation(material->shader, "Projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material->shader, "View"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material->shader, "Model"), 1, GL_FALSE, &m[0][0]);
	glUniform3f(glGetUniformLocation(material->shader, "Color"), material->color.r, material->color.g, material->color.b);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(material->shader, "texture_diffuse1"), 0);
	glBindTexture(GL_TEXTURE_2D, material->TEX);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);
}

void TexturedCube::update(double deltaTime) {

}

void TexturedCube::initCube(float size) {
	float half = size / 2.0f;

	//Vertices
	{
		//front
		vertices.push_back(glm::vec3(-half, -half, half));	//left, down, front
		vertices.push_back(glm::vec3(half, -half, half));	//right, down, front
		vertices.push_back(glm::vec3(half, half, half));	//right, top, front
		vertices.push_back(glm::vec3(-half, half, half));	//left, top, front
		//back
		vertices.push_back(glm::vec3(half, -half, -half));	//right, down, back
		vertices.push_back(glm::vec3(-half, -half, -half));	//left, down, back
		vertices.push_back(glm::vec3(-half, half, -half));	//left, top, back
		vertices.push_back(glm::vec3(half, half, -half));	//right, top, back
		//left
		vertices.push_back(glm::vec3(-half, -half, -half));	//left, down, back
		vertices.push_back(glm::vec3(-half, -half, half));	//left, down, front
		vertices.push_back(glm::vec3(-half, half, half));	//left, top, front
		vertices.push_back(glm::vec3(-half, half, -half));	//left, top, back
		//right
		vertices.push_back(glm::vec3(half, -half, half));	//right, down, front
		vertices.push_back(glm::vec3(half, -half, -half));	//right, down, back
		vertices.push_back(glm::vec3(half, half, -half));	//right, top, back
		vertices.push_back(glm::vec3(half, half, half));	//right, top, front
		//top
		vertices.push_back(glm::vec3(-half, half, half));	//left, top, front
		vertices.push_back(glm::vec3(half, half, half));	//right, top, front
		vertices.push_back(glm::vec3(half, half, -half));	//right, top, back
		vertices.push_back(glm::vec3(-half, half, -half));	//left, top, back
		//bottom
		vertices.push_back(glm::vec3(-half, -half, half));	//left, down, front
		vertices.push_back(glm::vec3(half, -half, half));	//right, down, front
		vertices.push_back(glm::vec3(half, -half, -half));	//right, down, back
		vertices.push_back(glm::vec3(-half, -half, -half));	//left, down, back
	}

	//indices
	{
		//front
		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(0); indices.push_back(2); indices.push_back(3);
		//back
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(4); indices.push_back(6); indices.push_back(7);
		//left
		indices.push_back(8); indices.push_back(9); indices.push_back(10);
		indices.push_back(8); indices.push_back(10); indices.push_back(11);
		//right
		indices.push_back(12); indices.push_back(13); indices.push_back(14);
		indices.push_back(12); indices.push_back(14); indices.push_back(15);
		//top
		indices.push_back(16); indices.push_back(17); indices.push_back(18);
		indices.push_back(16); indices.push_back(18); indices.push_back(19);
		//bottom
		indices.push_back(20); indices.push_back(23); indices.push_back(22);
		indices.push_back(20); indices.push_back(22); indices.push_back(21);
	}

	//Tex Coords
	{
		//Front
		texCoords.push_back(glm::vec2(0, 1)); 
		texCoords.push_back(glm::vec2(1, 1)); 
		texCoords.push_back(glm::vec2(1, 0)); 
		texCoords.push_back(glm::vec2(0, 0));
		//Back
		texCoords.push_back(glm::vec2(0, 1)); 
		texCoords.push_back(glm::vec2(1, 1)); 
		texCoords.push_back(glm::vec2(1, 0)); 
		texCoords.push_back(glm::vec2(0, 0));
		//Left
		texCoords.push_back(glm::vec2(0, 1)); 
		texCoords.push_back(glm::vec2(1, 1)); 
		texCoords.push_back(glm::vec2(1, 0)); 
		texCoords.push_back(glm::vec2(0, 0));
		//Right
		texCoords.push_back(glm::vec2(0, 1)); 
		texCoords.push_back(glm::vec2(1, 1));
		texCoords.push_back(glm::vec2(1, 0)); 
		texCoords.push_back(glm::vec2(0, 0));
		//Top
		texCoords.push_back(glm::vec2(0, 1)); 
		texCoords.push_back(glm::vec2(1, 1)); 
		texCoords.push_back(glm::vec2(1, 0)); 
		texCoords.push_back(glm::vec2(0, 0));
		//Bottom
		texCoords.push_back(glm::vec2(0, 1)); 
		texCoords.push_back(glm::vec2(0, 0)); 
		texCoords.push_back(glm::vec2(1, 0)); 
		texCoords.push_back(glm::vec2(1, 1));
	}
}

void TexturedCube::initBuffers() {
	//Begin
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);

	//Pass vertices
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Pass indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Pass texture coords
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &(texCoords[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*)0);
	
	//Finish
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}