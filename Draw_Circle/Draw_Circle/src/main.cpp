#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 셰이더 소스 코드
// ...
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

void main(void)
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0));
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main(void)
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // 주황색
}
)";

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Draw Circle", NULL, NULL);
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

	// Shader 셋업
	// -----------
	// 컴파일
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// 프로그램 생성
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 삭제 (링크 후 필요 없음)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex 셋업
	// -----------
	std::vector<float> circleVertices;

	// 중심 (0, 0)
	circleVertices.push_back(0.0f); // x
	circleVertices.push_back(0.0f); // y

	int numSegments = 100;
	float radius = 0.5f;

	for (int i = 0; i <= numSegments; ++i)
	{
		float angle = 2.0f * M_PI * i / numSegments;
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		circleVertices.push_back(x);
		circleVertices.push_back(y);
	}

	// VAO, VBO 셋업
	// -------------
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 버퍼 바인딩 및 데이터 설정
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Render Loop
	// ------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, numSegments + 2); // 중심 + 점들

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: 종료하고 이전에 할당된 모든 GLFW 리소스를 해제
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

//  GLFW 라이브러리를 사용하여 특정 키 입력을 감지하고 그에 따라 반응하는 함수
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Window 크기 변경시 호출 되는 callback 함수
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
