#include "Window.h"
#include "Events.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define WIDTH 1024
#define HEIGHT 720

float vertex[]{
    -0.5, 0.0, 0.0,
     0.0, 0.5, 0.0,
     0.5, 0.0, 0.0
};

int main(void)
{
    Window::createWindow(WIDTH, HEIGHT, "Window");
    Events::initialize();


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    while (Window::isWindowClose())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);      
        if (Events::justPressed(GLFW_KEY_ESCAPE))
            Window::closeWindow();

        if (Events::justPressed(GLFW_KEY_W))
            glClearColor(0.1, 1, 0, 1);
        if (Events::justPressed(GLFW_KEY_D))
            glClearColor(0.1, 1, 1, 1);
        if (Events::justPressed(GLFW_KEY_A))
            glClearColor(0.7, 0, 0, 1);
        if (Events::justPressed(GLFW_KEY_S))
            glClearColor(0.1, 0, 0.6, 1);

        /* Swap front and back buffers */
        Window::swapBuffer();
        /* Poll for and process events */
        Events::pullEvents();
    }

    Window::terminate();
    return 0;
}

