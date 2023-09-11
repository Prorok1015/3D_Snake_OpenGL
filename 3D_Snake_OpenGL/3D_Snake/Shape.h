#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shape
{
	unsigned int EBO;
	unsigned int VAO;
	unsigned int VBO;
public:
	glm::fvec3 pos;
	glm::fvec2 size;
	glm::fvec3 color{1.f, 0.f, 0.f};
public:
	//std::list<Shape> childs;

	std::vector<glm::fvec3> GetBounds();

	void Invalidate();

	virtual void draw();
	Shape();
	virtual ~Shape() {};
};

