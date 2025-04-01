#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Window ����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// GLFW�� �� �Լ��� ����Ͽ� â ũ�Ⱑ ������ ������ ȣ���ϵ��� �˷��ִ� �Լ�
	glfwMakeContextCurrent(window);
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
		
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window); // ���� �ݺ� �߿� �������ϴ� �� ���Ǵ� ���� ����(GLFW â�� �� �ȼ��� ���� ���� ���� ���Ե� ���� 2D ����)�� �����Ͽ� ȭ�鿡 ������� ǥ���ϴ� �Լ�
		glfwPollEvents(); // �̺�Ʈ(Ű���� �Է�, ���콺 �̵�)�� Ʈ���� �Ǵ��� Ȯ��, window ���� ������Ʈ, (�ݹ� �Լ��� ���� ��� ����)
	}

	// glfw: �����ϰ� ������ �Ҵ�� ��� GLFW ���ҽ��� ����
	glfwTerminate();
	return 0;
}

//  GLFW ���̺귯���� ����Ͽ� Ư�� Ű �Է��� �����ϰ� �׿� ���� �����ϴ� �Լ�
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Window ũ�� ����� ȣ�� �Ǵ� callback �Լ�
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}