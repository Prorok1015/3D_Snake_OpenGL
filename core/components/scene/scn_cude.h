#pragma once
#include <memory>
#include "scn_model.h"
namespace rnd {
	class Shader;
}

void init_cude();
void draw_cude(const rnd::Shader& ourShader);
void term_cude();

scene::Model create_cube();