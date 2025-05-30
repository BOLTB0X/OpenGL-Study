#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "sphere.h"

class Renderer {
public:
    Renderer(Window* window, Camera* camera);
    ~Renderer();
    // -------------------------------------
    void Init();
    void Render(float deltaTime);
    // -------------------------------------
private:
    Window* window;
    Camera* camera;
    // -------------------------------------
    Shader outlineShader;
    Shader amoghasiddhiShader;
    Shader spotShader;
    // -------------------------------------
    Model amoghasiddhi;
    Sphere lightSpot;
    // -------------------------------------
    float elapsedTime = 0.0f;
    float radius = 0.5f;
    float speed = 0.7f;
    // -------------------------------------
};

#endif