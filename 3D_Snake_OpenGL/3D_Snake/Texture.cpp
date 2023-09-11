#include "Texture.h"
#include "glad/glad.h"
#include "png_loader.h"
#include <iostream>

Texture::Texture(unsigned int id, int width, int height) : id(id), width(width), height(height) {
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

std::shared_ptr<Texture> Texture::load(std::string filename)
{
    if (png_result res = _png_load(filename.c_str())) {
        return std::make_shared<Texture>(res.id, res.width, res.height);
    }

    std::cerr << "Could not load texture " << filename << std::endl;
    return nullptr;
}
