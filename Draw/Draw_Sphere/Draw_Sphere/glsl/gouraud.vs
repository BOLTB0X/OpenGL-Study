#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 VertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void)
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vec3 norm = normalize(mat3(transpose(inverse(model))) * aNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightColor;
    vec3 result = diffuse * objectColor;
    VertexColor = result;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}