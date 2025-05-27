#include "mesh.h"
#include <iostream>

// 생성자
Mesh::Mesh(std::vector<Vertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<Texture> textures,
    Material material)
    : vertices(vertices), indices(indices), textures(textures), material(material)
{
    setupMesh();
}
// ---------------------------------------------------------------------------------
// 메쉬 렌더링
void Mesh::Draw(Shader& shader)
{
    // 텍스처 바인딩
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr = 0;
    unsigned int aoNr = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // 활성화
        std::string number;
        std::string name;

        switch (textures[i].type) {
        case TextureType::DIFFUSE:
            name = "texture_diffuse";
            number = std::to_string(diffuseNr++);
            break;
        case TextureType::SPECULAR:
            name = "texture_specular";
            number = std::to_string(specularNr++);
            break;
        case TextureType::NORMAL:
            name = "texture_normal";
            number = std::to_string(normalNr++);
            break;
        case TextureType::AO:
            name = "texture_ao";
            number = std::to_string(aoNr++);
            break;
        }
        //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        shader.setInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // 재질 속성 설정 (Ka, Kd, Ks 등)
    shader.setVec3("material.ambientColor", material.ambientColor);
    shader.setVec3("material.diffuseColor", material.diffuseColor);
    shader.setVec3("material.specularColor", material.specularColor);
    shader.setFloat("material.shininess", material.shininess);
    shader.setFloat("material.alpha", material.alpha);
    shader.setInt("material.illum", material.illum);

    shader.setBool("material.hasDiffuseMap", material.hasDiffuseMap);
    shader.setBool("material.hasSpecularMap", material.hasSpecularMap);
    shader.setBool("material.hasNormalMap", material.hasNormalMap);
    shader.setBool("material.hasAOMap", material.hasAOMap);

    // 그리기
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // 상태 초기화
    glActiveTexture(GL_TEXTURE0);
}
// ----------------------------------------------------------------------------------------
// VAO/VBO/EBO 세팅
void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}
// ----------------------------------------------------------------------------------------
