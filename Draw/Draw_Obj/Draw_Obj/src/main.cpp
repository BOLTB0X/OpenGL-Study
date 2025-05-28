#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <shader.h>
#include "camera.h"
#include "window.h"
#include "model.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void) {
    // init
	// -------------------------------------------
	// Window 객체 생성
	Window window(SCR_WIDTH, SCR_HEIGHT, "Draw_Buddha");
	// Camera 객체 생성 및 연결
	Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
	window.SetCamera(&camera);
	// 콜백 초기화 (마우스, 스크롤, 크기 조절 등)
	window.InitCallbacks();
	// -------------------------------------------

    Shader shader("glsl/buddha.vs", "glsl/buddha.fs");
    Model model("resources/buddha/buddhaPreaching.obj");

    stbi_set_flip_vertically_on_load(true);

    // 렌더링 루프
    // -------------------------------------------
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window.GetGLFWWindow())) {
        // 시간 계산
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        window.SetDeltaTime(deltaTime);

        // 입력 처리
        window.ProcessInput();

        // 화면 클리어
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::vec3 center = model.GetCenterPosition();
        modelMat = glm::translate(modelMat, -center);

        modelMat = glm::rotate(modelMat, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 3.0f));
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));

        shader.setMat4("model", modelMat);

        model.Draw(shader);

        // 버퍼 교환 및 이벤트 폴링
        glfwSwapBuffers(window.GetGLFWWindow());
        glfwPollEvents();
    }
    // -------------------------------------------

    window.~Window();
    camera.~Camera();
	return 0;
}
