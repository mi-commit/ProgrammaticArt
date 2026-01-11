#version 330 core

struct Material{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 camPos;
uniform Material material;
uniform Light light;

void main()
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));


	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPos);
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	vec3 diffuseLight = light.diffuse * diffuseFactor * vec3(texture(material.diffuse, TexCoord));

	vec3 viewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(-lightDirection, normal);  
	float spec = max(dot(viewDir, reflectDir), 0.0);
	spec = pow(spec, material.shininess);

	vec3 specular = spec*material.specular*light.specular;

	FragColor = vec4(diffuseLight + specular + ambient, 1.0);


};