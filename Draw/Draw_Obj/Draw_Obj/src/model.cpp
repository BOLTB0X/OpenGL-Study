#include "model.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// 생성자
Model::Model(const std::string& path)
{
    loadModel(path);
}
// ----------------------------------------------------------
// 렌더링
void Model::Draw(Shader& shader)
{

    for (auto& mesh : meshes) {
        mesh.Draw(shader);
    }
}
// ----------------------------------------------------------
glm::vec3 Model::GetCenterPosition() const {
    glm::vec3 minPos(FLT_MAX), maxPos(-FLT_MAX);
for (const auto& mesh : meshes) {
    for (const auto& vertex : mesh.vertices) {
        minPos = glm::min(minPos, vertex.Position);
        maxPos = glm::max(maxPos, vertex.Position);
    }
}
return (minPos + maxPos) / 2.0f;
}
// assimp 활용 모델 로드 메서드
void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}
// ----------------------------------------------------------
// 노드 처리
void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}
// ----------------------------------------------------------
// 메쉬 처리
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;

    // Vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        vertex.Normal = mesh->HasNormals()
            ? glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z)
            : glm::vec3(0.0f);

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);

            // tangent
            vertex.Tangent = glm::vec3(
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            );

            // bitangent
            vertex.Bitangent = glm::vec3(
                mesh->mBitangents[i].x,
                mesh->mBitangents[i].y,
                mesh->mBitangents[i].z
            );
        }
        else
            vertex.TexCoords = glm::vec2(0.0f);

        vertices.push_back(vertex);
    }

    // Index
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // Material
  
    aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, TextureType::SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps = loadMaterialTextures(mat, aiTextureType_HEIGHT, TextureType::NORMAL);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps = loadMaterialTextures(mat, aiTextureType_AMBIENT, TextureType::AO);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    material = loadMaterial(mat);

    material.hasDiffuseMap = !diffuseMaps.empty();
    material.hasSpecularMap = !specularMaps.empty();
    material.hasNormalMap = !normalMaps.empty();
    material.hasAOMap = !heightMaps.empty();
    
    return Mesh(vertices, indices, textures, material);
}
// ----------------------------------------------------------
// 텍스처 처리
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeEnum)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string filename = directory + "/" + std::string(str.C_Str());

        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(filename.c_str());
            texture.type = typeEnum;
            texture.path = filename;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}
// ----------------------------------------------------------
Material Model::loadMaterial(aiMaterial* mat)
{
    Material mtl;

    aiColor3D color(0.0f, 0.0f, 0.0f);
    float value;

    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_AMBIENT, color))
        mtl.ambientColor = glm::vec3(color.r, color.g, color.b);

    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
        mtl.diffuseColor = glm::vec3(color.r, color.g, color.b);

    if (AI_SUCCESS == mat->Get(AI_MATKEY_COLOR_SPECULAR, color))
        mtl.specularColor = glm::vec3(color.r, color.g, color.b);

    if (AI_SUCCESS == mat->Get(AI_MATKEY_SHININESS, value))
        mtl.shininess = value;

    if (AI_SUCCESS == mat->Get(AI_MATKEY_OPACITY, value))
        mtl.alpha = value;

    if (AI_SUCCESS == mat->Get(AI_MATKEY_SHADING_MODEL, value))
        mtl.illum = static_cast<int>(value);

    return mtl;
}
// ----------------------------------------------------------
unsigned int TextureFromFile(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
        {
            std::cout << "Unsupported image format for texture: " << path << std::endl;
            stbi_image_free(data);
            return 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
// ----------------------------------------------------------