#pragma once
#include <string>
#include <memory>

class Texture {
public:
	Texture(unsigned int id, int width, int height);
	~Texture();
	void bind();

	static std::shared_ptr<Texture> load(std::string_view filename);
private:

private:
	unsigned int id;
	int width;
	int height;
};
