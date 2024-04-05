#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>

class Shader {
public:
	unsigned int id;

public:
	Shader(unsigned int id);
	~Shader();

	unsigned ID() const { return id; }
	void use();
	void uniform_matrix(std::string name, glm::mat4 matrix);
	void uniform_float(std::string name, float val);

	static std::shared_ptr<Shader> load(std::string vertexFile, std::string fragmentFile, std::string geomFile);

	static std::shared_ptr<Shader> load(std::string vertexFile, std::string fragmentFile);
};

void prepare_global_structure();

void set_global_data(const glm::mat4& proj, const glm::mat4& view, float time);