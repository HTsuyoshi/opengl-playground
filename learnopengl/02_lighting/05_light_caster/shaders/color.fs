#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 direction;
	float cutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform Material material;
uniform Light light;

void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 norm = normalize(Normal);

	// Point Light (Infinite)
	vec3 lightDir = normalize(LightPos - FragPos);
	//vec3 lightDir = normalize(light.position - FragPos);

	// Directional Light
	//vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

	// Flashlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutoff - light.outerCutoff);
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// Point Light
	float distance = length(LightPos - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
