#include "png_loader.h"

#include "Image/Image.h"
#include <iostream>
#include "glad/glad.h"

png_result _png_load(const char* file)
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return png_result{ texture, t_width, t_height };
}
