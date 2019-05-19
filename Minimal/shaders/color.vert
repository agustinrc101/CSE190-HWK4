#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Color;

void main(){
    color = Color;    
    gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
}