#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


in vec3 Normal;
in vec3 FragPos;  

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform Material material;


void main()
{
	vec3 ambient = lightColor * material.ambient;


	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	vec3 diffuseLight = lightColor * (diffuseFactor * material.diffuse);

	vec3 viewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(-lightDirection, normal);  
	float spec = max(dot(viewDir, reflectDir), 0.0);
	spec = pow(spec, material.shininess);

	vec3 specular = spec*material.specular*lightColor;

	FragColor = vec4(diffuseLight + specular + ambient, 1.0);


};