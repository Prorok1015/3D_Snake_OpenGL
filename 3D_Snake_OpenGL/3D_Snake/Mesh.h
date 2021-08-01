#pragma once

#ifndef MESH
#define MESH
#include <glad/glad.h>

class Mesh
{
	GLuint VAO;
	GLuint VBO;
	size_t vertices;
public:
	Mesh(const float* buffer, size_t vertices, const int* attrs);
	~Mesh();

	void draw(GLuint primitive);
};

#endif // !MESH