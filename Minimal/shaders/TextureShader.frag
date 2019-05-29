#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 Color;
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
	vec4 c = texture(texture_diffuse1, TexCoords);			//Texture color
	c = c * vec4(Color.r, Color.g, Color.b, 1.0);			//Hue
	vec3 result = (ambient + diffuse + specular) * vec3(c);	//Phong
	FragColor = vec4(result, c.a);

	//TODO ambiance causes green hue
}