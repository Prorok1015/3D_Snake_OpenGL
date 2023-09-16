#pragma once
#include <array>
#include "../render/shader/shader.h"

class Shape
{
public:
	std::array<glm::fvec3, 4> bounds();
	void prepare_data();

	void draw();
	Shape();
	~Shape();

public:
	glm::fvec3 pos { 0.f, 0.f, 0.f };
	glm::fvec2 size { 0.f, 0.f };
	glm::fvec3 color { 1.f, 1.f, 1.f };
	std::unique_ptr<Shader> shader = nullptr;

private:
	unsigned int EBO = 0;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
};

