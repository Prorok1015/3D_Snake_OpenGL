#pragma once

#define GLFW_INCLUDE_NONE

class GLFWwindow;

class Window
{
public:	
	static GLFWwindow* window;
	static int width;
	static int height;

	static int createWindow(int width, int height, const char* title);
	static void terminate();
	static void swapBuffer();
	static bool isWindowClose();
	static void closeWindow(bool close = true);
	static void setCursorMode(int mode);
};

