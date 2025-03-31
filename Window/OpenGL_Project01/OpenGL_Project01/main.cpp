#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void) {
	// glfw: initialize and configure
	// --------------------------------

	glfwInit(); // GLFW initialize
	// GLFW configure
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window ����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// GLFW ���� �޼��� glfwSetFramebufferSizeCallback 
	// framebuffer_size_callback �Լ� �����ͷ� ���� ������ ũ��� ����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLAD �ʱ�ȭ: OpenGL �Լ� ������ ����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initailize GLAD" << std::endl;
		return -1;
	}

	// Render Loop
	// ------------
	while (!glfwWindowShouldClose(window)) { // glfwWindowShouldClose �Լ� : �� ���� �ݺ��� ���� �κп��� GLFW�� �������� ���õǾ����� Ȯ��
		glfwSwapBuffers(window);
		glfwPollEvents(); // �̺�Ʈ(Ű���� �Է�, ���콺 �̵�)�� Ʈ���� �Ǵ��� Ȯ��, window ���� ������Ʈ, (�ݹ� �Լ��� ���� ��� ����)
	}
	return 0;
}

// Window ũ�� ����� ȣ�� �Ǵ� callback �Լ�
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}