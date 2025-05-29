#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 TangentFragPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    TexCoords = aTexCoords;

    FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    TBN = mat3(T, B, N);

    TangentFragPos = TBN * FragPos;
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}