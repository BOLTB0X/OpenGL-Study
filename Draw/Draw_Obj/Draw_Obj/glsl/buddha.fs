#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

//uniform sampler2D texture_diffuse1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap;
    sampler2D aoMap;

    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;

    float shininess;
    float alpha;

    bool hasDiffuseTex;
    bool hasSpecularTex;
    bool hasNormalMap;
    bool hasAOMap;
};

uniform Material material;
uniform vec3 lightDir;
uniform vec3 lightColor;

void main()
{
    // ambient
    vec3 ambient = material.ambientColor * texture(material.diffuse, TexCoords).rgb;
    if (material.hasAOMap)
        ambient *= texture(material.aoMap, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, normalize(lightDir)), 0.0);
    vec3 diffuse = material.diffuseColor * diff;
    if (material.hasDiffuseTex)
        diffuse *= texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specularColor * spec;
    if (material.hasSpecularTex)
        specular *= texture(material.specular, TexCoords).rgb;

    // res
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, material.alpha);


    FragColor = texture(material.diffuse, TexCoords);
    //FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}