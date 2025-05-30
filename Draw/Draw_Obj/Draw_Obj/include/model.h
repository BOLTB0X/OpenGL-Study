#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <map>

#include <shader.h>
#include "mesh.h"

unsigned int TextureFromFile(const char* path, std::string& directory);

class Model {
public:
    Model(const std::string& path);
    //~Model();
    // ----------------------------------
    void Render(Shader& shader, glm::mat4 projection, glm::mat4 view, 
        glm::mat4 model, glm::vec3 position, glm::vec3 lightPos);
    void Render(Shader& shader, glm::mat4 projection, glm::mat4 view, 
        glm::mat4 model, float thickness);
    glm::vec3 GetCenterPosition() const;
    // ----------------------------------
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    // -----------------------------------
    void loadModel(const std::string& path);
    void draw(Shader& shader);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    // ------------------------------------
};
// ----------------------------
#endif