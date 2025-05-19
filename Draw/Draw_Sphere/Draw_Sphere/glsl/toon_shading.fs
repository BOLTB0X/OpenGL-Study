#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 intensity = vec3(0.0, 0.0, 0.0);
    if (diff > 0.95)
        intensity = vec3(1.0, 0.5, 0.5);
    else if (diff > 0.5)
        intensity = vec3(0.6, 0.3, 0.3);
    else if (diff > 0.25)
        intensity = vec3(0.4, 0.2, 0.2);
    else
        intensity = vec3(0.1, 0.1, 0.1);

    vec3 result = intensity * lightColor * objectColor;
    FragColor = vec4(result, 1.0);
}
