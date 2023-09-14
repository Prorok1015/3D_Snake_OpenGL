#pragma once
#include <vector>
#include <array>
#include <string_view>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../render/texture/Texture.h"

class Shape
{
	unsigned int EBO;
	unsigned int VAO;
	unsigned int VBO;
public:
	glm::fvec3 pos;
	glm::fvec2 size;
	glm::fvec3 color{1.f, 0.f, 0.f};
	std::array<glm::fvec2, 4> textureSize{glm::fvec2{1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}, {0.f, 1.f}};
	std::shared_ptr<Texture> texture = nullptr;
public:
	//std::list<Shape> childs;

	std::vector<glm::fvec3> GetBounds();

	void LoadTexture(std::string_view res);

	void Invalidate();

	virtual void draw();
	Shape();
	virtual ~Shape() {};
};

