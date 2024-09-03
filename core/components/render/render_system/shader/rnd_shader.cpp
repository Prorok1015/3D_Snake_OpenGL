#include "rnd_shader.h"

rnd::Shader::~Shader() {
}

void rnd::Shader::use() {
	shader_impl->use();
}

void rnd::Shader::uniform(std::string_view name, glm::mat4 matrix)
{
	shader_impl->uniform(name, matrix);
}

void rnd::Shader::uniform(std::string_view name, glm::vec3 vec)
{
	shader_impl->uniform(name, vec);
}

void rnd::Shader::uniform(std::string_view name, glm::vec2 vec)
{
	shader_impl->uniform(name, vec);
}

void rnd::Shader::uniform(std::string_view name, int val)
{
	shader_impl->uniform(name, val);
}

void rnd::Shader::uniform(std::string_view name, float val)
{
	shader_impl->uniform(name, val);
}

