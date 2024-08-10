#include "rnd_shader.h"

render::Shader::~Shader() {
}

void render::Shader::use() {
	shader_impl->use();
}

void render::Shader::uniform(std::string_view name, glm::mat4 matrix)
{
	shader_impl->uniform(name, matrix);
}

void render::Shader::uniform(std::string_view name, int val)
{
	shader_impl->uniform(name, val);
}

