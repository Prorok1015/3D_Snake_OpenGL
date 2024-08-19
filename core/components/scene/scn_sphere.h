#pragma once
#include <common.h>
namespace rnd{
	class Shader;
}

void init_sphere();
void draw_sphere(const rnd::Shader& ourShader);
void term_sphere();