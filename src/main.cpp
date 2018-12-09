#include "cube.h"
#include "audio_processor.h"

#include <GLFW/glfw3.h>
#include <iostream>

int screen_width=800, screen_height=600;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

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

    AudioProcessor audioProcessor;
    
    std::vector<std::unique_ptr<RenderObject>> renderObjects;
    float lastUpdateTime = 0.0;
    float lastTime = 0.0;
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        float time = (float) glfwGetTime();
        float dt = time - lastTime;
        for (auto& obj : renderObjects) {
            obj->update(dt, screen_width, screen_height);
        }
        
        for (auto& obj : renderObjects) {
            obj->render();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        for (auto& obj : renderObjects) {
            obj->translate(exp::nullopt, 0, 0.5);
        }
        
        int scaledTime = (int)(time*100.0);
        if (lastUpdateTime != scaledTime && scaledTime % 1 == 0) {
            lastUpdateTime = scaledTime;
            
            float peak = audioProcessor.get_last_peak_avg();
            for (int i=0; i < 10; i++) {
                
                color_t color = {(float)i * 0.1f, (float)i * 0.1f, 0};
                auto cube = std::make_unique<Cube>(color);
                cube->set_translation(-10.0 + 2.0*i, -5.0 + peak * 1000.0, -50.0);
                renderObjects.emplace_back(std::move(cube));
            }
            
            if (renderObjects.size() > 1000) {
                int toDelete = renderObjects.size() - 1000;
                renderObjects.erase(renderObjects.begin(), renderObjects.begin() + toDelete);
            }
        }
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}
