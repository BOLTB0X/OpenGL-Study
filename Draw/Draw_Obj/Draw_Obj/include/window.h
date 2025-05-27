#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

class Window {
public:
	Window(unsigned int width, unsigned int height, const char* title);
	~Window();
	// ----------------------------------------------------------------------
	GLFWwindow* GetGLFWWindow() const;
	void SetCamera(Camera* cam); // camera 포인터 전달
	void ProcessInput();
	void SetDeltaTime(float delta);
	void InitCallbacks(); // 콜백 등록
	// ----------------------------------------------------------------------
private:
	GLFWwindow* window;
	unsigned int width, height;
	const char* title;
	Camera* camera;
	// mouse input
	bool firstMouse = true;
	float lastX, lastY;
	float deltaTime = 0.0f;
	// ----------------------------------------------------------------------
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	// ----------------------------------------------------------------------
};

#endif
