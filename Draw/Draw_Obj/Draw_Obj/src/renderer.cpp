#include "renderer.h"
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 생성자
Renderer::Renderer(Window* window, Camera* camera) :
    window(window), 
    camera(camera),
    outlineShader("glsl/outline.vs", "glsl/outline.fs"),
    amoghasiddhiShader("glsl/buddha.vs", "glsl/toon.fs"),
    spotShader("glsl/lightSpot.vs", "glsl/lightSpot.fs"),
    amoghasiddhi("resources/amoghasiddhi/mia_031182_Amoghasiddhi_64k.obj"),
    lightSpot(1.0f, 20, 20)
{}
// ----------------------------------------------------------------------
Renderer::~Renderer()
{
    outlineShader.~Shader();
    amoghasiddhiShader.~Shader();
    spotShader.~Shader();
}
// ----------------------------------------------------------------------
void Renderer::Init()
{
    stbi_set_flip_vertically_on_load(true);
}
// ----------------------------------------------------------------------
void Renderer::Render(float deltaTime)
{
    elapsedTime += deltaTime;

    // 배경
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)window->width / window->height, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 lightPos = glm::vec3(sin(elapsedTime * speed) * radius, 0.0f, cos(elapsedTime * speed) * radius);
    model = glm::translate(model, -amoghasiddhi.GetCenterPosition());

    // outline
    amoghasiddhi.Render(outlineShader, projection, view, model, 0.005f);
    // main
    amoghasiddhi.Render(amoghasiddhiShader, projection, view, model, camera->Position, lightPos);
    // lightSpot
    lightSpot.Render(spotShader, view, projection, lightPos);
}
// ----------------------------------------------------------------------