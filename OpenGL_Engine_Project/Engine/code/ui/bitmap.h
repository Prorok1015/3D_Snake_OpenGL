#pragma once
#include <array>
#include "../render/texture/texture.h"
#include "../render/shader/shader.h"

class Bitmap
{
public:
	Bitmap();
	~Bitmap();

	void prepare_data();
	void draw();

	std::array<glm::fvec3, 4> bounds();
	void load_texture(std::string_view res);

public:
	glm::fvec3 pos{ 0.f, 0.f, 0.f };
	glm::fvec2 size{ 0.f, 0.f };
	glm::fvec3 color{ 1.f, 1.f, 1.f };
	std::array<glm::fvec2, 4> textureSize{ glm::fvec2{1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}, {0.f, 1.f} };

private:/*
	std::shared_ptr<Texture> texture = nullptr;*/
	std::shared_ptr<Shader> shader = nullptr;

private:
	unsigned int EBO = 0;
	unsigned int VAO = 0;
	unsigned int VBO = 0;
};