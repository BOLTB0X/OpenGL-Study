#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Sphere {
public:
    Sphere(float radius, int sectorCount, int stackCount);
    ~Sphere();
    // ------------------------------------------------------------------------
    void draw(void) const;
    // ------------------------------------------------------------------------
private:
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
