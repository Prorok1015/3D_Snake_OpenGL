#include "Texture.h"
#include "../../common/common.h"
#include "../../serialize/image_loader.h"

Texture::Texture(unsigned int id, int width, int height) : id(id), width_(width), height_(height) {
}

Texture::~Texture() {
	//glDeleteTextures(1, &id);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

Texture Texture::load_from_res(std::shared_ptr<res::Texture> res)
{
    FILTERING minfiltering = Texture::FILTERING::LINEAR_MIPMAP;
    FILTERING magfiltering = Texture::FILTERING::LINEAR; 
    WRAPPING wrapping = Texture::WRAPPING::REPEAT;

    GLsizei t_width = res->size().x;
    GLsizei t_height = res->size().y;
    GLenum format = 0;
    if (res->channels() == 1)
        format = GL_RED;
    else if (res->channels() == 3)
        format = GL_RGB;
    else if (res->channels() == 4)
        format = GL_RGBA;
    GLubyte* image_data = res->data();
    GLuint texture;
    glGenTextures(1, &texture);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texture);
    CHECK_GL_ERROR();
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minfiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magfiltering);
    CHECK_GL_ERROR();
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapping);
    CHECK_GL_ERROR();

    glTexImage2D(GL_TEXTURE_2D, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, image_data);
    CHECK_GL_ERROR();
    glGenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();

    return Texture{ texture, t_width, t_height };
}

std::shared_ptr<Texture> Texture::load(std::string_view filename)
{
    return Texture::load(filename, FILTERING::NEAREST, WRAPPING::REPEAT);
}

std::shared_ptr<Texture> Texture::load(std::string_view filename, FILTERING filtering, WRAPPING wrapping)
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

        return std::make_shared<Texture>(texture, t_width, t_height);
    }

    std::cerr << "Could not load texture " << filename << std::endl;
    ASSERT_FAIL("Could not load texture");
    return nullptr;
}

Texture Texture::loadClean(std::string_view filename, FILTERING minfiltering, FILTERING magfiltering, WRAPPING wrapping)
{
    if (std::unique_ptr<Image> img = load_image(filename.data())) {
        GLsizei t_width = img->width();
        GLsizei t_height = img->height();
        GLenum format = 0;
        if (img->channels_count() == 1)
            format = GL_RED;
        else if (img->channels_count() == 3)
            format = GL_RGB;
        else if (img->channels_count() == 4)
            format = GL_RGBA;
        GLubyte* image_data = img->pop_data();
        GLuint texture;
        glGenTextures(1, &texture);
        CHECK_GL_ERROR();
        glBindTexture(GL_TEXTURE_2D, texture);
        CHECK_GL_ERROR();
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minfiltering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magfiltering);
        CHECK_GL_ERROR();
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapping);
        CHECK_GL_ERROR();

        glTexImage2D(GL_TEXTURE_2D, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, image_data);
        CHECK_GL_ERROR();
        glGenerateMipmap(GL_TEXTURE_2D);
        CHECK_GL_ERROR();
        glBindTexture(GL_TEXTURE_2D, 0);
        CHECK_GL_ERROR();

        return Texture{ texture, t_width, t_height };
    }

    std::cerr << "Could not load texture " << filename << std::endl;
    ASSERT_FAIL("Could not load texture");
    return {0, 0, 0};
}
