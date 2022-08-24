#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "shaderP.hpp"

// float uMoveX = 0.0f;
// float uMoveY = 0.0f;
glm::vec3 uPos;
glm::vec4 uColor;

float length = 0.1f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
        glfwSetWindowShouldClose(window, 1);

    if(action != GLFW_PRESS) return;

    // if(key == GLFW_KEY_UP && action == GLFW_PRESS && uMoveY < 1.0f)
    //     uMoveY += length;
    // if(key == GLFW_KEY_DOWN && action == GLFW_PRESS && uMoveY > -1.0f)
    //     uMoveY -= length;

    // if(key == GLFW_KEY_LEFT && action == GLFW_PRESS && uMoveX > -1.0f)
    //     uMoveX -= length;
    // if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS && uMoveX < 1.0f)
    //     uMoveX += length;

    if(key == GLFW_KEY_UP && uPos.y < 1.0f)
        uPos.y += length;
    if(key == GLFW_KEY_DOWN && uPos.y > -1.0f)
        uPos.y -= length;

    if(key == GLFW_KEY_LEFT && uPos.x > -1.0f)
        uPos.x -= length;
    if(key == GLFW_KEY_RIGHT && uPos.x < 1.0f)
        uPos.x += length;
}



int main(){

    if(glfwInit() != GL_TRUE){
        std::cout << "Error glfwinit" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "Ne olur", nullptr, nullptr);
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

    // sp.addUniform("uMoveX");
    // sp.addUniform("uMoveY");
    sp.addUniform("uPos");
    sp.addUniform("uColor");
    sp.use();

    float points[] = {
        -length / 2.0f, -length / 2.0f, 0.0f,
         length / 2.0f, -length / 2.0f, 1.0f,
         length / 2.0f,  length / 2.0f, 0.3f,
        -length / 2.0f,  length / 2.0f, 0.0f
    };
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    uint32_t vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
    glEnableVertexAttribArray(0);

    uint32_t ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window)){

        glClearColor(0.0f, 0.5f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        sp.use();
            // sp.setUniformValueFloat("uMoveX", uMoveX);
            // sp.setUniformValueFloat("uMoveY", uMoveY);
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                // glDrawArrays(GL_TRIANGLES, 0, 6);
                    sp.setUniformValueVec3("uPos", uPos);
                    sp.setUniformValueVec4("uColor", glm::vec4(1, 0, 0, 1.0f));
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                    sp.setUniformValueVec3("uPos", uPos + glm::vec3(length, length, 0.0f));
                    sp.setUniformValueVec4("uColor", glm::vec4(0, 0, 1, 1.0f));
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                    sp.setUniformValueVec3("uPos", uPos + glm::vec3(-length, -length, 0.0f));
                    sp.setUniformValueVec4("uColor", uColor + glm::vec4(0, 1, 0, 1.0f));
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
