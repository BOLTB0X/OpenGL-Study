#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"
#include "Sphere.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 lightPos = glm::vec3(3.0f, 3.0f, 10.0f);
glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(void)
{
	glfwInit(); // GLFW initialize
	// GLFW configure
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Window 생성
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Draw Sphere", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD 초기화: OpenGL 함수 포인터 관련
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initailize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sphere 셋업
	Sphere sphere(1.0f, 50, 50);

	// Shader 셋업
	//Shader sphereShader("glsl/vertex_shader.vs", "glsl/fragment_shader.fs");

	// Gouraud Shading
	//Shader sphereShader("glsl/gouraud.vs", "glsl/gouraud.fs");

	// Flat Shading
	//Shader sphereShader("glsl/flat.vs", "glsl/flat.fs");

	// Toon Shading
	// ===================================================
	Shader sphereShader("glsl/vertex_shader.vs", "glsl/toon_shading.fs");
	Shader outlineShader("glsl/outline.vs", "glsl/outline.fs");
	// ===================================================

	// 투영, 뷰, 모델 매트릭스 설정
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

	// Render Loop
	// ------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Phong , Gouraud , Flat Shading
		//sphere.draw(sphereShader, model, view, projection, lightPos, viewPos, lightColor);


		// Toon Shading
		// ===================================================
		// 1. 윤곽선
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelOutline = glm::scale(model, glm::vec3(1.05f));
		sphere.draw(outlineShader, modelOutline, view, projection);

		// 2. 구
		glCullFace(GL_BACK);
		sphere.draw(sphereShader, model, view, projection, lightPos, viewPos, lightColor);
		// ===================================================

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	sphereShader.~Shader();
	outlineShader.~Shader();
	sphere.~Sphere();

	glfwTerminate();
	return 0;
}
//  GLFW 라이브러리를 사용하여 특정 키 입력을 감지하고 그에 따라 반응하는 함수
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
// -----------------------------------------------------------------------------
// Window 크기 변경시 호출 되는 callback 함수
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// -----------------------------------------------------------------------------
