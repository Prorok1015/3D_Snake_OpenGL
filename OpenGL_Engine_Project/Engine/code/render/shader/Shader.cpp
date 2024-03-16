#include "shader.h"
#include "../../common/common.h"
#include "../../resource/res_resource_system.h"
#include "../../resource/res_resource_shader.h"
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(unsigned int id) : id(id) {
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::uniform_matrix(std::string name, glm::mat4 matrix)
{
	GLuint transformLoc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::shared_ptr<Shader> Shader::load(std::string vertexFile, std::string fragmentFile)
{

	auto vertexCode = res::get_system().require_resource_shader(res::Tag::make(vertexFile));
	auto fragmentCode = res::get_system().require_resource_shader(res::Tag::make(fragmentFile));

	const GLchar* vShaderCode = vertexCode->c_str();
	const GLchar* fShaderCode = fragmentCode->c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		ASSERT_FAIL("SHADER::VERTEX: compilation failed");
		return nullptr;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		ASSERT_FAIL("SHADER::FRAGMENT: compilation failed");
		return nullptr;
	}

	// Shader Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		ASSERT_FAIL("SHADER::PROGRAM: linking failed");
		return nullptr;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return std::make_shared<Shader>(id);
}
