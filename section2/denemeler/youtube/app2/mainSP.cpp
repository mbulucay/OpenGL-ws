#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "shaderP.hpp"
#include "square.hpp"

#define  len 0.06

glm::vec3 uPos;
glm::vec4 uColor;

float length = 0.1f;
std::vector<Square*> squares;

void addTail(std::vector<Square*>& squares);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
        glfwSetWindowShouldClose(window, 1);

    // if(action != GLFW_PRESS) return;
    if(squares.size() == 0) return;

    if(key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT)
        squares.at(0)->setDir(key);
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        addTail(squares);
}

void draw(ShaderProgram& sp, std::vector<Square*>& squares){
    
    for(auto& s : squares){
        sp.setUniformValueVec3("uPos", s->getPos());
        sp.setUniformValueVec4("uColor", s->getColor());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}


void move(std::vector<Square*>& squares){

    for(auto& s : squares){
        s->move();
    }

    for(int i=squares.size()-1 ; i>0 ; --i){
        squares.at(i)->setDir(squares.at(i-1)->getDir());
    }
}

void addTail(std::vector<Square*>& squares){

    if(squares.size() == 0){
        squares.push_back(new Square(glm::vec2(0,0), len));
        return;
    }

    glm::vec3 lastPos = squares.at(squares.size()-1)->getPos();
    int lastDir = squares.at(squares.size()-1)->getDir();

    switch (lastDir)
    {
        case GLFW_KEY_UP:
            squares.push_back(new Square(glm::vec2(lastPos.x, lastPos.y - len), len));
            break;
        case GLFW_KEY_DOWN:
            squares.push_back(new Square(glm::vec2(lastPos.x, lastPos.y + len), len));
            break;
        case GLFW_KEY_LEFT:
            squares.push_back(new Square(glm::vec2(lastPos.x + len, lastPos.y), len));
            break;
        case GLFW_KEY_RIGHT:
            squares.push_back(new Square(glm::vec2(lastPos.x - len, lastPos.y), len));
            break;
    }
}

int main(){

    srand(time(NULL));

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
    
    addTail(squares);
    addTail(squares);
    addTail(squares);

    ShaderProgram sp;
    sp.attachShader("./vertexS.glsl", GL_VERTEX_SHADER);
    sp.attachShader("./fragmentS.glsl", GL_FRAGMENT_SHADER);
    sp.link();
    sp.validate();

    sp.addUniform("uPos");
    sp.addUniform("uColor");
    sp.use();

    float points[] = {
        -len / 2.0f, -len / 2.0f, 0.0f,
         len / 2.0f, -len / 2.0f, 1.0f,
         len / 2.0f,  len / 2.0f, 0.3f,
        -len / 2.0f,  len / 2.0f, 0.0f
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
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                    move(squares);
                    draw(sp, squares);

                    std::this_thread::sleep_for(std::chrono::milliseconds(25));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
