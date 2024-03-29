#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
    gl_Position = Projection * View * Model * vec4(aPos, 1.0f);

	Normal = mat3(transpose(inverse(Model))) * aNormal;
	FragPos = vec3(Model * vec4(aPos, 1.0));
}