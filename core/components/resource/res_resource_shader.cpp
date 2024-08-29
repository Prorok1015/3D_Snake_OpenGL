#include "res_resource_shader.h"
#include "res_resource_system.h"
#include <fstream>
#include <sstream>

res::Shader::Shader(const Tag& tag)
	: Resource(tag)
{
	std::string path = ResourceSystem::get_absolut_path(tag);
	if (path.empty()) {
		return;
	}

	try {
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::badbit);
		shaderFile.open(path, std::ifstream::in);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		body_ = shaderStream.str();
	}
	catch (const std::ifstream::failure&) {
		ASSERT_FAIL("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
}
