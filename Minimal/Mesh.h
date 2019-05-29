#ifndef H_MESH
#define H_MESH
#pragma once

#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Material.h"
#include "Shaders.h"
#include "Light.h"

using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}
	~Mesh() {
		indices.clear();
		vertices.clear();
		textures.clear();

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO2);
	}

	// render the mesh
	void draw(glm::mat4 headPose, glm::mat4 projection, glm::mat4 M, Material * mat) {
		glm::mat4 m = M * glm::mat4(1);
		GLuint shader = 0;
		if(mat->TEX != 0)	shader = mat->shader;
		else				shader = Shaders::getColorShader();
		
		glUseProgram(shader);

		if (textures.size() > 0) {
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
				// retrieve texture number (the N in diffuse_textureN)
				string number;
				string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++); // transfer unsigned int to stream
				else if (name == "texture_normal")
					number = std::to_string(normalNr++); // transfer unsigned int to stream
				else if (name == "texture_height")
					number = std::to_string(heightNr++); // transfer unsigned int to stream

														 // now set the sampler to the correct texture unit
				glUniform1i(glGetUniformLocation(mat->shader, (name + number).c_str()), i);
				// and finally bind the texture
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(mat->shader, "texture_diffuse1"), 0);
			glBindTexture(GL_TEXTURE_2D, mat->TEX);
		}

		glm::vec3 lightP = Light::getLightPosition();
		glm::vec3 lightC = Light::getLightColor();
		glm::vec3 cameraPos = Light::getCameraPos();

		glUniform1i(glGetUniformLocation(shader, "TexCoords"), 0);
		glUniformMatrix4fv(glGetUniformLocation(shader, "Projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "View"), 1, GL_FALSE, &headPose[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "Model"), 1, GL_FALSE, &m[0][0]);
		glUniform3f(glGetUniformLocation(shader, "Color"), mat->color.r, mat->color.g, mat->color.b);
		glUniform3f(glGetUniformLocation(shader, "LightPos"), lightP.x, lightP.y, lightP.z);
		glUniform3f(glGetUniformLocation(shader, "LightColor"), lightC.r, lightC.g, lightC.b);
		glUniform3f(glGetUniformLocation(shader, "CameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform1f(glGetUniformLocation(shader, "SpecularStrenght"), mat->specular);
		glUniform1f(glGetUniformLocation(shader, "AmbianceStrength"), mat->ambiance);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		//glActiveTexture(GL_TEXTURE0);
	}

private:
	/*  Render data  */
	unsigned int VBO, EBO, VBO2;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh() {
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBO2);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
};
#endif
