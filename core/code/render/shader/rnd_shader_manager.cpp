#include "rnd_shader_manager.h"
#include "../../resource/res_resource_system.h"
#include "../../resource/res_resource_shader.h"

render::ShaderManager::ShaderManager()
{
}

render::ShaderManager::~ShaderManager()
{
}

render::Shader render::ShaderManager::use(std::string_view shader) const
{
	auto it = _cache.find(shader);
	if (it == _cache.end()) {
		auto new_shader = load(shader.data());
		new_shader.use();
		new_shader.uniform("texture_diffuse1", 0);
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

void render::ShaderManager::uniform(const std::string_view shader, const std::string_view field, int val) const
{
	auto sh = use(shader);
	sh.uniform(field, val);
	unuse();
}

void render::ShaderManager::init_global_uniform() const
{
	_matrices = std::make_shared<render::driver::gl::uniform_buffer>(sizeof(GlobalUniform), 0);
}

void render::ShaderManager::update_global_uniform(const GlobalUniform& val) const
{
	_matrices->set_data(val.projection, offsetof(GlobalUniform, projection));
	_matrices->set_data(val.view, offsetof(GlobalUniform, view));
	_matrices->set_data(val.time, offsetof(GlobalUniform, time));
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
	CHECK_GL_ERROR();
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	CHECK_GL_ERROR();
	glCompileShader(vertex);
	CHECK_GL_ERROR();
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	CHECK_GL_ERROR();
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		egLOG("shader/load", "Vertex Shader: \n{0}", infoLog);
		return -1;
	}

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	CHECK_GL_ERROR();
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	CHECK_GL_ERROR();
	glCompileShader(fragment);
	CHECK_GL_ERROR();
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	CHECK_GL_ERROR();
	if (!success) {
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		egLOG("shader/load", "Fragment Shader: \n{0}", infoLog);
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
	CHECK_GL_ERROR();
	glAttachShader(id, vertex);
	CHECK_GL_ERROR();
	glAttachShader(id, fragment);
	CHECK_GL_ERROR();
	//glAttachShader(id, geom);
	glLinkProgram(id);
	CHECK_GL_ERROR();

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	CHECK_GL_ERROR();
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		//glDeleteShader(geom);

		egLOG("shader/load", "{0}", infoLog);
		return -1;
	}

	glDeleteShader(vertex);
	CHECK_GL_ERROR();
	glDeleteShader(fragment);
	CHECK_GL_ERROR();
	//glDeleteShader(geom);

	egLOG("shader/load", "Success loading shader '{0}'", name);

	return Shader(id);
}
