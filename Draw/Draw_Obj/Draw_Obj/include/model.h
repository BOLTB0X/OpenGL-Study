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
    // ----------------------------------
    void Draw(Shader& shader);
    glm::vec3 GetCenterPosition() const;
    // ----------------------------------
private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    // -----------------------------------
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    // ------------------------------------
};
// ----------------------------
#endif