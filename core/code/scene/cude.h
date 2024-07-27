#pragma once
#include <memory>
#include "model.h"
namespace render {
	class Shader;
}

void init_cude();
void draw_cude(const render::Shader& ourShader);
void term_cude();

scene::Model create_cube();