#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "window.h"
#include "renderer.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void) {
    // init
	// -------------------------------------------
	// Window 객체 생성
	Window window(SCR_WIDTH, SCR_HEIGHT, "Draw Amoghasiddhi");
	// Camera 객체 생성 및 연결
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	window.SetCamera(&camera);
	// 콜백 초기화 (마우스, 스크롤, 크기 조절 등)
	window.InitCallbacks();
	// -------------------------------------------

    Renderer renderer(&window, &camera);
    renderer.Init();

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

        // 렌더
        renderer.Render(deltaTime);

        // 버퍼 교환 및 이벤트 폴링
        glfwSwapBuffers(window.GetGLFWWindow());
        glfwPollEvents();
        // ---------------------------------------------------
    }
    // -------------------------------------------

    window.~Window();
    camera.~Camera();
    renderer.~Renderer();
	return 0;
}
