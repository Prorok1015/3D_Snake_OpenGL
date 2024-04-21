#include "rnd_shader_manager.h"
#include "../../resource/res_resource_system.h"
#include "../../resource/res_resource_shader.h"

render::ShaderManager::ShaderManager()
{
}

render::ShaderManager::~ShaderManager()
{
	glDeleteBuffers(1, &uboMatrices);
}

render::Shader render::ShaderManager::use(std::string_view shader) const
{
	auto it = _cache.find(shader);
	if (it == _cache.end()) {
		auto new_shader = load(shader.data());
		new_shader.use();
		return _cache[shader] = new_shader;		
	}
	it->second.use();
	return it->second;
}

void render::ShaderManager::unuse() const
{
	glUseProgram(0);
}

void render::ShaderManager::uniform(const std::string_view shader, const std::string_view field, glm::mat4 val) const
{
	auto sh = use(shader);
	sh.uniform_matrix(field, val);
	unuse();
}

void render::ShaderManager::init_global_uniform() const
{
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(float));
}

void render::ShaderManager::update_global_uniform(const GlobalUniform& val) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(val.projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(val.view));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(float), std::addressof(val.time));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

render::Shader render::ShaderManager::load(const std::string& name) const
{
	auto vertexCode = res::get_system().require_resource<res::Shader>(res::Tag::make(name + ".vert"), true);
	auto fragmentCode = res::get_system().require_resource<res::Shader>(res::Tag::make(name + ".frag"), true);
	auto geomCode = res::get_system().require_resource<res::Shader>(res::Tag::make(name + ".geom"), true);

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
		egLOG("shader/load", "Vertex Shader: \n{}", infoLog);
		return -1;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		egLOG("shader/load", "Fragment Shader: \n{}", infoLog);
		return -1;
	}

	// TEMPORARY OFF
	//// Geometry Shader
	//geom = glCreateShader(GL_GEOMETRY_SHADER);
	//glShaderSource(geom, 1, &gShaderCode, nullptr);
	//glCompileShader(geom);
	//glGetShaderiv(geom, GL_COMPILE_STATUS, &success);
	//if (!success) {
	//	glGetShaderInfoLog(geom, 512, nullptr, infoLog);
	//	egLOG("shader/load", "Geometry Shader: \n{}", infoLog);
	//	return -1;
	//}

	// Shader Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	//glAttachShader(id, geom);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		//glDeleteShader(geom);

		egLOG("shader/load", "{}", infoLog);
		return -1;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	//glDeleteShader(geom);

	egLOG("shader/load", "Success loading shader {}", name);
	return Shader(id);
}
