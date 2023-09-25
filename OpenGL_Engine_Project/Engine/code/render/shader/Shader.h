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

	static std::unique_ptr<Shader> load(std::string vertexFile, std::string fragmentFile);
};

