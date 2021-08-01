#include "png_loader.h"

#include "Image/Image.h"
#include <iostream>
#include "glad/glad.h"
#include "Texture.h"

int _png_load(const char* file, int* width, int* height) 
{
    Image img(file);

    GLsizei t_width, t_height;
    GLubyte* image_data;

    GLuint texture;
    GLenum alpha;

    t_width = img.getWidth();
    t_height = img.getHeight();
    image_data = img.getData();
    if (img.getChannels() == 3)
    {
        alpha = GL_RGB;
    }
    else
    {
        alpha = GL_RGBA;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0,
        alpha, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    *width = t_width;
    *height = t_height;

    return texture;
}

Texture* load_texture(std::string filename) {
    int width, height;

    GLuint texture = _png_load(filename.c_str(), &width, &height);
    if (texture == 0) {
        std::cerr << "Could not load texture " << filename << std::endl;
        return nullptr;
    }    
    return new Texture(texture, width, height);
}