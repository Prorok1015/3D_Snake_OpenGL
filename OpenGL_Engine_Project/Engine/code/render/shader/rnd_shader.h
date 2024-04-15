#pragma once
#include "../../common/common.h"

namespace render
{
	class Shader {
	public:
		unsigned int id = 0;

	public:
		Shader() = default;
		Shader(unsigned int id);
		~Shader();

		unsigned ID() const { return id; }
		void use();
		void uniform_matrix(const std::string_view name, glm::mat4 matrix);
		void uniform_float(std::string name, float val);

		void uniform(std::string_view name, int val) const
		{
			glUniform1i(glGetUniformLocation(id, name.data()), val);
			CHECK_GL_ERROR();
		}

		void uniform(std::string_view name, float val) const
		{
			glUniform1f(glGetUniformLocation(id, name.data()), val);
			CHECK_GL_ERROR();
		}

		static std::shared_ptr<Shader> load(std::string vertexFile, std::string fragmentFile, std::string geomFile);

		static std::shared_ptr<Shader> load(std::string vertexFile, std::string fragmentFile);
	};
}
