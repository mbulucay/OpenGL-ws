#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>


const GLint width = 800, height = 600;

GLuint VAO, VBO, shader;


void create_triangle(){

    GLfloat vertices[] = {
    //     x     y    z
        -1.0f, -1.0, 0.0f, // p1
         1.0f, -1.0, 0.0f, // p2
         0.0f,  1.0, 0.0f  // p3
    };

    // bu fonksiyonla grafik karti vertex arrayi olsuturuyoruz VAO olarak id veriyor
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

/* 
    _____________________________
    |                           |
    |    -x, y           x , y  |
    |                           |
    |                           |
    |             0,0           |
    |                           |
    |    -x, -y          x, -y  |
    |                           |
    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
*/


int main(int argc, char* argv[]){

    if(!glfwInit()){
        std::cout << "ERROR glfwinit" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);

    if(!window){
        std::cout << "ERROR window" << std::endl;
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if(GLEW_OK != glewInit()){
        std::cout << "Error GLEW init" << std::endl;
        glfwTerminate();
        return 3;
    }

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();


    return 0;
}