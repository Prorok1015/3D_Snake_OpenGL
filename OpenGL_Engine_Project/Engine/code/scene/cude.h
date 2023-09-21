#pragma once
#include <memory>
class Shader;

void init_cude();
void draw_cude(std::unique_ptr<Shader>& ourShader);
void delete_cude();