#include <iostream>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "../include/shaderP.hpp"


int main(){

    if(glfwInit() != GL_TRUE){
        std::cout << "Error glfwinit" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ne olur", nullptr, nullptr);
    if(window == nullptr){
        glfwTerminate();
        return 1;
    }

    int bWidth, bHeight;
    glfwGetFramebufferSize(window, &bWidth, &bHeight);
    glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::cout << "Glew init failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }
    glViewport(0, 0, bWidth, bHeight);  
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);


    ShaderProgram sp;
    sp.attachShader("./resources/shaders/vertexS.glsl", GL_VERTEX_SHADER);
    sp.attachShader("./resources/shaders/fragmentS.glsl", GL_FRAGMENT_SHADER);
    sp.link();
    sp.validate();
    sp.use();

    float points[] = {
        -0.2f, -0.2f, 0.0f,
         0.3f,  0.5f, 1.0f,
         0.5f, -0.5f, 0.3f
    };

    uint32_t vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
    glEnableVertexAttribArray(0);


    while(!glfwWindowShouldClose(window)){

        glClearColor(0.7f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
