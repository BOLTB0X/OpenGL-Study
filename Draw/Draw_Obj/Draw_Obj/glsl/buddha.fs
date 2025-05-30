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
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); 

    // 광원, 뷰 벡터
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

    // 디퓨즈
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * texture(texture_diffuse1, TexCoords).rgb;

    // 스페큘러
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * texture(texture_specular1, TexCoords).rgb;

    // Ambient Occlusion
    float ao = texture(texture_occlusion1, TexCoords).r;

    vec3 ambient = texture(texture_diffuse1, TexCoords).rgb * ao;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}