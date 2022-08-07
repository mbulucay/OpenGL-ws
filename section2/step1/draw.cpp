#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

int main(int argc, char* argv[]){

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Ne olur", nullptr, nullptr);
    if(window == nullptr){
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);


    uint32_t vsId = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fsId = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vs = 
    "#version 330 core\n            \
        layout (location = 0) in vec3 inPos; \
        void main(){ \
            gl_Position = vec4(inPos, 1.0f); \
        }";

    // locationun esitlendigi sey vao nun slot numarasi oda vboya gidiyor
    std::string fs = 
    "#version 330 core \n \
        out vec4 color; \
        void main(){ \
            color = vec4(1.0f, 0.0f, 1.0f, 1.0f); \
        }";
    
    const char* vsSrc = vs.c_str();
    glShaderSource(vsId, 1, &vsSrc, nullptr);
    glCompileShader(vsId);

    const char* fsSrc = fs.c_str();
    glShaderSource(fsId, 1, &fsSrc, nullptr);
    glCompileShader(fsId);
    
    uint32_t pid = glCreateProgram();
    glAttachShader(pid, vsId);
    glAttachShader(pid, fsId);

    glLinkProgram(pid);
    glValidateProgram(pid);

    glDeleteShader(vsId);
    glDeleteShader(fsId);

    float points[] = {
        -0.2f, -0.2f, 0.0f,
         0.3f,  0.5f, 1.0f,
         0.5f, -0.5f, 0.3f
    };

    /////////////////////// ONEMLI ////////////
    uint32_t vao;
    uint32_t vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

    glEnableVertexAttribArray(0);

    // https://www.youtube.com/watch?v=p9S9jjLuEO8&list=PLIM5iw4GHbNW2nleNK5XIvyhZzGiaiBfp&index=10&ab_channel=KayhanAyar t = 8.00
    /////////////////////////////

    while(!glfwWindowShouldClose(window)){
    
        glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // eger sabit ise bunlari while iicnde yazmana gerek yok ama pratik bu sekilde
        glUseProgram(pid);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}