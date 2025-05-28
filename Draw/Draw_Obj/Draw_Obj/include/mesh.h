#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <shader.h>

// 버텍스
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent = glm::vec3(0.0f);
    // bitangent
    glm::vec3 Bitangent = glm::vec3(0.0f);
};
// ----------------------------
// 텍스처
struct Texture
{
    unsigned int id;
    std::string type;
    //TextureType type;
    std::string path;
};
// ----------------------------
class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    // ----------------------------
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures); // 생성자
    void Draw(Shader& shader);
    // ----------------------------
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    // ----------------------------
};
// ----------------------------
#endif