#pragma once

#define GLFW_INCLUDE_NONE

class GLFWwindow;

class Window
{
public:	
	static GLFWwindow* window;
	static int width;
	static int height;

	static int initialize(int width, int height, const char* title);
	static void terminate();
	static void swapBuffers();
	static bool isShouldClose();
	static void setShouldClose(bool close = true);
	static void setCursorMode(int mode);
};

