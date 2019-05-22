#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 Color;

void main(){
    FragColor = texture(texture_diffuse1, TexCoords) * vec4(Color.r, Color.g, Color.b, 1.0);
}