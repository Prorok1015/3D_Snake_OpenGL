#include "Texture.h"
#include "../../common/common.h"
#include "../../serialize/image_loader.h"

Texture::Texture(unsigned int id, int width, int height) : id(id), width_(width), height_(height) {
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

std::unique_ptr<Texture> Texture::load(std::string_view filename)
{
    return Texture::load(filename, FILTERING::NEAREST, WRAPPING::REPEAT);
}

std::unique_ptr<Texture> Texture::load(std::string_view filename, FILTERING filtering, WRAPPING wrapping)
{   
    if (std::unique_ptr<Image> img = load_image(filename.data())) {
        GLsizei t_width = img->width();
        GLsizei t_height = img->height();
        GLenum alpha = img->channels_count() == 3 ? GL_RGB : GL_RGBA;
        GLubyte* image_data = img->pop_data();
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapping);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, alpha, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return std::make_unique<Texture>(texture, t_width, t_height);
    }

    std::cerr << "Could not load texture " << filename << std::endl;
    ASSERT_FAIL("Could not load texture");
    return nullptr;
}
