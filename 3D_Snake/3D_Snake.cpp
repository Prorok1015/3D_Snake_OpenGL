#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include "Window.h"
#include "Events.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"


#define WIDTH 1024
#define HEIGHT 720

float vertex[]{
       -1.0f,-1.0f, 0.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
       -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
       -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

int main(void)
{
    Window::createWindow(WIDTH, HEIGHT, "Window");
    Events::initialize();

    Shader* shader = load_shader("./res/vertex.glslv", "./res/fragment.glslf");
    if (shader == nullptr)
    {
        std::cout << "error";
        system("pause");
        Window::terminate();
        return 1;
    }

    Texture* texture = load_texture("./res/test2.png");
    if (texture == nullptr)
    {
        std::cout << "error";
        system("pause");
        delete shader;
        Window::terminate();
        return 1;
    }


    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glClearColor(0.6f, 0.62f, 0.65f, 1);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    Camera* camera = new Camera(vec3(0, 0, 1), radians(90.0f));

    mat4 model(1.0f);
    model = translate(model, vec3(0.5f, 0, 0));

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    float speed = 5;


    /* Loop until the user closes the window */
    while (Window::isWindowClose())
    {
        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);      
        if (Events::justPressed(GLFW_KEY_ESCAPE))
            Window::closeWindow();

        if (Events::justPressed(GLFW_KEY_TAB)) {
            Events::toogleCursor();
        }

        if (Events::isPressed(GLFW_KEY_W)) {
            camera->position += camera->front * delta * speed;
        }
        if (Events::isPressed(GLFW_KEY_S)) {
            camera->position -= camera->front * delta * speed;
        }
        if (Events::isPressed(GLFW_KEY_D)) {
            camera->position += camera->right * delta * speed;
        }
        if (Events::isPressed(GLFW_KEY_A)) {
            camera->position -= camera->right * delta * speed;
        }
        if (Events::justCliced(GLFW_MOUSE_BUTTON_1))
            glClearColor(0.1, 1, 1, 1);



        if (Events::_cursor_locked) {
            camY += -Events::deltaY / Window::height * 2;
            camX += -Events::deltaX / Window::height * 2;

            if (camY < -radians(89.0f)) {
                camY = -radians(89.0f);
            }
            if (camY > radians(89.0f)) {
                camY = radians(89.0f);
            }

            camera->rotation = mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix("projview", camera->getProjection()* camera->getView());
        texture->bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        Window::swapBuffer();
        /* Poll for and process events */
        Events::pullEvents();
    }

    delete shader;
    delete texture;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    Window::terminate();
    return 0;
}

