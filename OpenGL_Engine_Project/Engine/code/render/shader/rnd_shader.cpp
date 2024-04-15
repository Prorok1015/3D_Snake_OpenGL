#include "rnd_shader.h"
#include "../../resource/res_resource_system.h"
#include "../../resource/res_resource_shader.h"
#include <fstream>
#include <sstream>

render::Shader::Shader(unsigned int id) : id(id) {
}

render::Shader::~Shader() {
	//glDeleteProgram(id);
}

void render::Shader::use() {
	glUseProgram(id);
}

void render::Shader::uniform_matrix(const std::string_view name, glm::mat4 matrix)
{
	GLuint transformLoc = glGetUniformLocation(id, name.data());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void render::Shader::uniform_float(std::string name, float val)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), val);
}

std::shared_ptr<render::Shader> render::Shader::load(std::string vertexFile, std::string fragmentFile, std::string geomFile)
{
	auto vertexCode = res::get_system().require_resource<res::Shader>(res::Tag::make(vertexFile), true);
	auto fragmentCode = res::get_system().require_resource<res::Shader>(res::Tag::make(fragmentFile), true);
	auto geomCode = res::get_system().require_resource<res::Shader>(res::Tag::make(geomFile), true);

	const GLchar* vShaderCode = vertexCode->c_str();
	const GLchar* fShaderCode = fragmentCode->c_str();
	const GLchar* gShaderCode = geomCode->c_str();

	GLuint vertex, fragment, geom;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		egLOG("shader/load", "{}", infoLog);
		return nullptr;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		egLOG("shader/load", "{}", infoLog);
		return nullptr;
	}

	// Geometry Shader
	geom = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geom, 1, &gShaderCode, nullptr);
	glCompileShader(geom);
	glGetShaderiv(geom, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(geom, 512, nullptr, infoLog);
		egLOG("shader/load", "{}", infoLog);
		return nullptr;
	}

	// Shader Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glAttachShader(id, geom);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteShader(geom);

		egLOG("shader/load", "{}", infoLog);
		return nullptr;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geom);

	egLOG("shader/load", "Success loading shader {}; {}; {}", vertexFile, fragmentFile, geomFile);
	return std::make_shared<Shader>(id);
}

std::shared_ptr<render::Shader> render::Shader::load(std::string vertexFile, std::string fragmentFile)
{

	auto vertexCode = res::get_system().require_resource<res::Shader>(res::Tag::make(vertexFile), true);
	auto fragmentCode = res::get_system().require_resource<res::Shader>(res::Tag::make(fragmentFile), true);

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
		//std::cerr << infoLog << std::endl;
		egLOG("shader/load", "{}", infoLog);
		//ASSERT_FAIL("SHADER::VERTEX: compilation failed");
		return nullptr;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		//std::cerr << infoLog << std::endl;
		egLOG("shader/load", "{}", infoLog);
		//ASSERT_FAIL("SHADER::FRAGMENT: compilation failed");
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
		//std::cerr << infoLog << std::endl;
		egLOG("shader/load", "{}", infoLog);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		//ASSERT_FAIL("SHADER::PROGRAM: linking failed");
		return nullptr;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	egLOG("shader/load", "Success loading shader {}; {}", vertexFile, fragmentFile);
	return std::make_shared<Shader>(id);
}