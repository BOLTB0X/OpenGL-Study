#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Sphere {
public:
    Sphere(float radius, int sectorCount, int stackCount);
    ~Sphere();
    // ------------------------------------------------------------------------
    void draw(Shader& shader,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection) const;
    // ------------------------------------------------------------------------
    void draw(Shader& shader,
              const glm::mat4& model,
              const glm::mat4& view,
              const glm::mat4& projection,
              const glm::vec3& lightPos,
              const glm::vec3& viewPos,
              const glm::vec3& lightColor) const;
    // ------------------------------------------------------------------------
private:
    void createSphere(void);
    void normalizeVertices(void);
    void setupMesh(void);
    // ------------------------------------------------------------------------
    float radius;
    int sectorCount, stackCount;
    glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.3f);
    // ------------------------------------------------------------------------
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    // ------------------------------------------------------------------------
    unsigned int VAO, VBO, EBO;
    // ------------------------------------------------------------------------
};
#endif
