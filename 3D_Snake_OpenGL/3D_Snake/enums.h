#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

enum class CursorMode
{
	Disable = GLFW_CURSOR_DISABLED,
	Normal = GLFW_CURSOR_NORMAL,
	Hidden = GLFW_CURSOR_HIDDEN
};

enum class DrawMode
{
	Line = GL_LINE,
	Triangle = GL_TRIANGLES
};