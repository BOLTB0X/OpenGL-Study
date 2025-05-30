#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 TangentFragPos;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in mat3 TBN;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_occlusion1;

void main()
{
    // 노멀 맵 및 정규화
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); 

    // 광원, 시점 방향
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    // 툰
    float diff = max(dot(normal, lightDir), 0.0);
    float toonDiffuse = 0.0;

    if (diff > 0.95)
        toonDiffuse = 1.0;
    else if (diff > 0.5)
        toonDiffuse = 0.6;
    else if (diff > 0.25)
        toonDiffuse = 0.3;
    else
        toonDiffuse = 0.1;

    // 툰 하이라이트
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    float toonSpecular = 0.0;

    if (spec > 0.8)
        toonSpecular = 1.0;
    else if (spec > 0.3)
        toonSpecular = 0.4;
    else
        toonSpecular = 0.0;

    //ambient
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    float ao = texture(texture_occlusion1, TexCoords).r;
    vec3 ambient = color * ao * 0.2;

    vec3 result = ambient + color * toonDiffuse + vec3(toonSpecular);
    FragColor = vec4(result, 1.0);
}