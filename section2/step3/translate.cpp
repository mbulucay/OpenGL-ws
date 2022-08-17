#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint width = 800, height = 600;

GLuint VAO, VBO, VBO2;
uint32_t pid;
uint32_t uniformModel;

bool direction = true;
float offset = 0.0f;
float maxOffset = 0.07f;
float increment = 0.01f;

static const char* vertex_shader2 = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 pos;

    void main(){
        gl_Position = vec4(pos.x , pos.y, pos.z, 1.0f);
    })glsl";

static const char* vertex_shader = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 pos;

    uniform mat4 model;

    void main(){
        gl_Position = model * vec4(pos.x , pos.y, pos.z, 1.0f);
    })glsl";
/* 
    model ile carpiyoruz cunku translate edebilcek sekilde ayarladik ve mevcut pozisyonu kaydiracagiz

    eger birim matrix olursa yer degistirmez ama biz offset i x yerine koyduk bu da demek oluyorki x yerine koydumuz degisken
    sayesinde sekil x eksininde hareket saglaycak koydugumuz degiskene gore
*/


static const char* fragment_shader = R"glsl(
    #version 330 core
    out vec4 color;
    void main(){
        color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    })glsl";


uint32_t add_shader(uint32_t type, const char* src){

    uint32_t shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int errorResult;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &errorResult);
    if(errorResult == GL_FALSE){
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length);                  // stack de dinamik olrak yer ayirmani sagliyor
        glGetShaderInfoLog(shaderId, length, &length, message);
        
        std::cerr << "Failed to compile " << 
            ((type == GL_VERTEX_SHADER) ? ("Vertex") : ("Fragment")) << " shader!" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(shaderId);
    }

    return shaderId;
}

void create_program(){

    uint32_t vs2Id = add_shader(GL_VERTEX_SHADER, vertex_shader2);
    uint32_t vsId = add_shader(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fsId = add_shader(GL_FRAGMENT_SHADER, fragment_shader);

    pid = glCreateProgram();
    if(pid == GL_FALSE){
        std::cout << "Program creation error" << std::endl;
        return;
    }
    // glAttachShader(pid, vs2Id);
    glAttachShader(pid, vsId);
    glAttachShader(pid, fsId);
    
    glLinkProgram(pid);

    int errorResult;
    glGetProgramiv(pid, GL_LINK_STATUS, &errorResult);
    if(!errorResult){
        char str[1000];
        int l;
        glGetProgramInfoLog(pid, sizeof(str), &l, str);
        std::cerr << "Error Linking program" << std::endl;
        return;
    }

    glValidateProgram(pid);

    glGetProgramiv(pid, GL_VALIDATE_STATUS, &errorResult);
    if(!errorResult){
        char str[1000];
        int l;
        glGetProgramInfoLog(pid, sizeof(str), &l, str);
        std::cerr << "Error validating program" << std::endl;
        return;
    }

    uniformModel = glGetUniformLocation(pid, "model");

    glDeleteShader(vs2Id);
    glDeleteShader(vsId);
    glDeleteShader(fsId);
}

void create_shape(){

    GLfloat vertices[] = {
        //     x     y    z
        -0.5f, -0.5, 0.0f, // p1
         0.5f, -0.5, 0.0f, // p2
        -0.5f,  0.5, 0.0f, // p3
         0.5f,  0.5, 0.0f  // p4
    };

    GLfloat vertices2[] = {
        //     x     y    z
         0.5f,  0.5, 0.0f,  // p4
         0.5f, -0.5, 0.0f, // p2
        -0.5f,  0.5, 0.0f, // p3
    };


    // bu fonksiyonla grafik karti vertex arrayi olsuturuyoruz VAO olarak id veriyor
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
            // glEnableVertexAttribArray(0);
        
        glGenBuffers(1, &VBO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
            // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
            // glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}


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

    create_shape();
    create_program();

    float val = 0.0f; 

    while(!glfwWindowShouldClose(window)){

        if(val >= 1.0f)
            val = -1.0f;
        val += 0.01;

        glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // eger sabit ise bunlari while iicnde yazmana gerek yok ama pratik bu sekilde
        glUseProgram(pid);

            glBindVertexArray(VAO);
                glEnableVertexAttribArray(0);
                glBindVertexBuffer(0, VBO, 0, 3 * sizeof(float));
                    
                    // glUniform1f(uniformModel, offset);
                    glm::mat4 model(1.0f);
                    model = glm::translate(model, glm::vec3(offset, val, 0.0f));

                    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                    //            degistirelcek degisken, 1, GL_FALSE, degistilecek veri kaynagi
                    glDrawArrays(GL_TRIANGLES, 0, 3);
                
                for(int i=0; i<10000; i++)
                    for(int j =0; j<1000; ++j){

                    }

                glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glBindVertexBuffer(0, VBO2, 0, 3 * sizeof(float));
                    glDrawArrays(GL_TRIANGLES, 0, 3);

                for(int i=0; i<10000; i++)
                    for(int j =0; j<1000; ++j){

                    }

            glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(direction){
            offset += increment;
        }else{
            offset -= increment;
        }
        if(abs(offset) >= maxOffset){
            direction = !direction;
        }
    }

    glfwTerminate();


    return 0;
}