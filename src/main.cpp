#include "cube.h"

#include <GLFW/glfw3.h>
#include <iostream>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int screen_width=800, screen_height=600;
static void resize_callback(GLFWwindow* window, int width, int height)
{
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;
    
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(screen_width, screen_height, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, resize_callback);
    
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);
    
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    std::vector<std::unique_ptr<RenderObject>> renderObjects;
    
    for (int i=0; i < 10; i++) {
    
        auto cube = std::make_unique<Cube>();
        cube->set_translation(-25.0 + 5.0*i, exp::nullopt, -50.0);
        renderObjects.emplace_back(std::move(cube));
    }
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        float dt = (float) glfwGetTime();
        for (auto& obj : renderObjects) {
            obj->update(dt, screen_width, screen_height);
        }
        
        for (auto& obj : renderObjects) {
            obj->render();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
