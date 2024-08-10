#include "rnd_gl_shader.h"
#include "open_gl_specific.h"
#include <glm/gtc/type_ptr.hpp>

render::driver::gl::shader::~shader()
{
	glDeleteProgram(id);
}

void render::driver::gl::shader::uniform(std::string_view name, glm::mat4 val)
{
	GLuint transformLoc = glGetUniformLocation(id, name.data());
	CHECK_GL_ERROR();
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(val));
	CHECK_GL_ERROR();
}

void render::driver::gl::shader::uniform(std::string_view name, int val)
{
	glUniform1i(glGetUniformLocation(id, name.data()), val);
	CHECK_GL_ERROR();
}

void render::driver::gl::shader::uniform(std::string_view name, float val)
{
	glUniform1f(glGetUniformLocation(id, name.data()), val);
	CHECK_GL_ERROR();
}

void render::driver::gl::shader::use()
{
	glUseProgram(id);
	CHECK_GL_ERROR();
}
