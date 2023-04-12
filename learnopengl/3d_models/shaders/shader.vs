#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 transform;
uniform vec3 lightPos;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;

void main()
{
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0f);
    Normal = mat3(transpose(inverse(view * model * transform))) * aNormal;
    FragPos = vec3(view * model * transform * vec4(aPos, 1.0));
	LightPos = vec3(view * vec4(lightPos, 1.0));
	TexCoord = aTexCoord;
}
