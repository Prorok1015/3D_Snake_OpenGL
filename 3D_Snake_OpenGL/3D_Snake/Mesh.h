#pragma once

#ifndef MESH
#define MESH
#include <glad/glad.h>

class Mesh
{
	GLuint VAO;
	GLuint VBO;
	size_t vertices;
	size_t vertexSize;
public:
	Mesh(const float* buffer, size_t vertices, const int* attrs);
	~Mesh();
	void reload(const float* buffer, size_t vertices);
	void draw(GLuint primitive);
};

#endif // !MESH