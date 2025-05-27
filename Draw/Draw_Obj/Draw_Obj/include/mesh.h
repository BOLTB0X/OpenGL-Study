#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <shader.h>

// 버텍스
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};
// ----------------------------
// 텍스처 종류
enum TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    AO
};
// 텍스처
struct Texture {
    unsigned int id;
    TextureType type;
    std::string path;
};
// ----------------------------
// 제질
struct Material {
    glm::vec3 ambientColor;   // Ka
    glm::vec3 diffuseColor;   // Kd
    glm::vec3 specularColor;  // Ks
    float shininess;     // Ns
    float alpha;         // d
    int illum;           // illum

    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasNormalMap;
    bool hasAOMap;
};
// ----------------------------
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;
    // ----------------------------
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material); // 생성자
    void Draw(Shader& shader);
    // ----------------------------
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    // ----------------------------
};
// ----------------------------
#endif