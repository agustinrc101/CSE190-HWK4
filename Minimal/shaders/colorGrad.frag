#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 uv;

uniform sampler2D texture_diffuse1;
uniform vec3 Color;
uniform vec3 Color2;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 CameraPos;
uniform float SpecularStrenght;
uniform float AmbianceStrength;

void main(){
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	vec3 viewDir = normalize(CameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	//Ambient 
	vec3 ambient = AmbianceStrength * LightColor;

	//Diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor;

	//Specular 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = SpecularStrenght * spec * LightColor;

	//Fragment Color
	vec4 c = texture(texture_diffuse1, uv);
	vec3 gradient = mix(Color, Color2, uv.y);
	c *= vec4(gradient, 1);

	FragColor = c;
}