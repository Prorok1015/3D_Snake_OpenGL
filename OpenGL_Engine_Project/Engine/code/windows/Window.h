#pragma once
struct GLFWwindow;
enum class CursorMode;


class Window
{
public:	
	static GLFWwindow* window;
	static int width;
	static int height;

	static int initialize(int width, int height, const char* title);
	static void terminate();
	static void swap_buffers();
	static bool isShouldClose();
	static void setShouldClose(bool close = true);
	static void setCursorMode(CursorMode mode);
};

