#include "Lines.h"


Lines::~Lines(){
	if(material != NULL) delete(material);
	indices.clear();
	vertices.clear();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Lines::draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 M, Material * mat) {
	//Rebind buffers
	bindBuffers();

	//Begin draw
	glm::mat4 m = M * toWorld;

	glUseProgram(mat->shader);

	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "Projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "View"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(mat->shader, "Model"), 1, GL_FALSE, &m[0][0]);
	glUniform3f(glGetUniformLocation(mat->shader, "Color"), mat->color.r, mat->color.g, mat->color.b);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Lines::draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 M) {
	//Rebind buffers
	bindBuffers();

	//Begin draw
	glm::mat4 m = M * toWorld;

	glUseProgram(material->shader);

	glUniformMatrix4fv(glGetUniformLocation(material->shader, "Projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material->shader, "View"), 1, GL_FALSE, &headPose[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(material->shader, "Model"), 1, GL_FALSE, &m[0][0]);
	glUniform3f(glGetUniformLocation(material->shader, "Color"), material->color.r, material->color.g, material->color.b);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
}

void Lines::bindBuffers(){
	//Rebind buffers
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
}

void Lines::updateInitialPosition(glm::vec3 pos) {
	if (vertices.size() > 1)
		vertices[0] = pos;
}

void Lines::addVertex(glm::vec3 v) {
	if (vertices.empty()) {
		vertices.push_back(v);
		return;
	}
	
	vertices.push_back(v);
	indices.push_back(0);
	indices.push_back((GLuint)vertices.size() - 1);
}