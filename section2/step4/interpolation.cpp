#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const char* vertex_shader = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 pos;

    uniform float uCoefficient;
    uniform mat4 uModel;

    void main(){
        gl_Position = uModel * vec4(uCoefficient * pos.x , uCoefficient * pos.y, uCoefficient * pos.z, 1.0f);
    })glsl";


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

uint32_t pid;
uint32_t uCoefficient;
uint32_t uModel;
void create_program(){

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

    uCoefficient = glGetUniformLocation(pid, "uCoefficient");
    uModel = glGetUniformLocation(pid, "uModel");

    glDeleteShader(vsId);
    glDeleteShader(fsId);
}



int main(){

    if(glfwInit() != GL_TRUE){
        std::cout << "Error glfwinit" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 800, "Ne olur", nullptr, nullptr);
    if(window == nullptr){
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if(GLEW_OK != glewInit()){
        std::cout << "Error GLEW init" << std::endl;
        glfwTerminate();
        return 3;
    }

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

    float points[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };
    uint32_t indices[] = {
        0, 1, 2
    };

    create_program();

    uint32_t vao, vbo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);
            glEnableVertexAttribArray(0);
    
    uint32_t ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window)){
        
        glClearColor(0.1f, 0.2f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       glUseProgram(pid);
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                    
                    glUniform1f(uCoefficient, 1.0f);
                    glm::mat4 model(1.0f);
                    
                    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
                    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

                    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));



                    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

                glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        glUseProgram(0);

        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;
}
