#include "bitmap.h"
#include "../common/common.h"
#include "../windows/window.h"

Bitmap::Bitmap()
{
    shader = Shader::load("./res/bitmap.glslv", "./res/bitmap.glslf");
    if (!shader) {
        ASSERT_FAIL("Bitmap's shader not create");
        //Window::terminate();
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Bitmap::~Bitmap()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Bitmap::prepare_data()
{
    //auto rect = bounds();

    //const int halfWidth = Window::width / 2;
    //const int halfHeight = Window::height / 2;

    //constexpr int column_count = 3 + 3 + 2;
    //constexpr int row_count = 4;
    //float vertices[column_count * row_count] = { 0 };

    //for (int i = 0; i < row_count; ++i) {
    //    int vecId = i * column_count;
    //    vertices[vecId] = (rect[i].x - halfWidth) / halfWidth;
    //    vertices[vecId + 1] = -(rect[i].y - halfHeight) / halfHeight;
    //    vertices[vecId + 2] = rect[i].z;
    //    vertices[vecId + 3] = color.r;
    //    vertices[vecId + 4] = color.g;
    //    vertices[vecId + 5] = color.b;

    //    if (!texture) {
    //        continue;
    //    }
    //    vertices[vecId + 6] = size.x / texture->width() * textureSize[i].x;
    //    vertices[vecId + 7] = size.y / texture->height() * textureSize[i].y;
    //}

    //static unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    //// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, column_count * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, column_count * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //// texture attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, column_count * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    //// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);
}

void Bitmap::draw()
{
    if (texture) {
        texture->bind();
    }
    if (shader) {
        shader->use();
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

std::array<glm::fvec3, 4> Bitmap::bounds()
{
    std::array<glm::fvec3, 4> bounds{};

    //lopRight
    bounds[0] = { pos.x + size.x, pos.y, pos.z };
    //bottomRight
    bounds[1] = { pos.x + size.x, pos.y + size.y, pos.z };
    //bottomLeft
    bounds[2] = { pos.x, pos.y + size.y, pos.z };
    //lopLeft
    bounds[3] = { pos.x, pos.y, pos.z };
    return bounds;
}

void Bitmap::load_texture(std::string_view res)
{
	texture = Texture::load(res);
}