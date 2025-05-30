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
    void Render(Shader& shader, const glm::mat4 view, 
        const glm::mat4 projection, const glm::vec3 lightPos);
    // ------------------------------------------------------------------------
private:
    void draw(Shader& shader) const;
    void createSphere(void);
    void normalizeVertices(void);
    void setupMesh(void);
    // ------------------------------------------------------------------------
    float radius;
    int sectorCount, stackCount;
    // ------------------------------------------------------------------------
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    // ------------------------------------------------------------------------
    unsigned int VAO, VBO, EBO;
    // ------------------------------------------------------------------------
};
#endif