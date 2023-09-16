#include "image_loader.h"
#include "../common/common.h"
#include <Image/Image.h>

png_result load_image(const char* file)
{
    Image img;
    img.flip(Image::ImageFlip::VERTICAL);
    if (!img.open(file)) {
        ASSERT_FAIL("Cant open file");
        return {};
    }

    GLsizei t_width, t_height;
    GLubyte* image_data;

    GLuint texture;
    GLenum alpha;

    t_width = img.width();
    t_height = img.height();
    image_data = img.pop_data();
    if (img.channels_count() == 3) {
        alpha = GL_RGB;
    } else {
        alpha = GL_RGBA;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, alpha, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return png_result{ texture, t_width, t_height };
}
