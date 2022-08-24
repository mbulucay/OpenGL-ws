#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "shaderP.hpp"

float uMoveX = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if(key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, 1);

    if(key == GLFW_KEY_LEFT)
        uMoveX -= 0.011;
    
    if(key == GLFW_KEY_RIGHT)
        uMoveX += 0.011;
}


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

    glfwSetKeyCallback(window, key_callback);

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
    sp.attachShader("./vertexS.glsl", GL_VERTEX_SHADER);
    sp.attachShader("./fragmentS.glsl", GL_FRAGMENT_SHADER);
    sp.link();
    sp.validate();

    sp.addUniform("uMoveX");
    sp.use();

    float points[] = {
        -0.2f, -0.2f, 0.0f,
         0.3f,  0.5f, 1.0f,
         0.5f, -0.5f, 0.3f
         
        -0.2f, -0.2f, 0.0f,
         0.3f,  0.5f, 1.0f,
         0.6f, -0.5f, 0.3f
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

        glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
            sp.setUniformValueFloat("uMoveX", uMoveX);
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
