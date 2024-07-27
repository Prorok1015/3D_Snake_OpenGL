#pragma once
#include "../common/common.h"
namespace render{
	class Shader;
}

void init_sphere();
void draw_sphere(const render::Shader& ourShader);
void term_sphere();