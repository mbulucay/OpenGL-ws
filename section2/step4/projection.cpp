#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <thread>
#include <chrono>

const float toRad = 3.141592f / 180.0f;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
        glfwSetWindowShouldClose(window, 1);
}


static const char* vertex_shader = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 pos;

    out vec4 vColor;

    uniform mat4 uModel;
    uniform mat4 uProjection;

    void main(){
        
        gl_Position =  uProjection * uModel * vec4(pos.x , pos.y, pos.z, 1.0f);
        vColor = vec4( clamp(pos.x, 0.0f, 1.0f),  clamp(pos.y, 0.0f, 1.0f), clamp(pos.z, 0.0f, 1.0f) , 1.0f);

    })glsl";


static const char* fragment_shader = R"glsl(
    #version 330 core
    
    in vec4 vColor;
    out vec4 color;

    void main(){
        // color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
        color = vColor;
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
uint32_t uModel;
uint32_t uProjection;
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

    uModel = glGetUniformLocation(pid, "uModel");
    uProjection = glGetUniformLocation(pid, "uProjection");

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
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    
    int bW, bH;
    glfwGetFramebufferSize(window, &bW, &bH);

    if(GLEW_OK != glewInit()){
        std::cout << "Error GLEW init" << std::endl;
        glfwTerminate();
        return 3;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bW, bH);

    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

    float points[] = {
        -1.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  1.0f, 
         1.0f, -1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f, -1.0f, -1.0f
    };
    uint32_t indices[] = {
        0, 3, 1,
        1, 3, 2,
        // 2, 3, 0,
        2, 3, 4,
        3, 0, 4,
        0, 1, 2
    };

    create_program();

    uint32_t vao, vbo, ibo;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);
            glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::mat4 projection = glm::perspective(45.0f, (float)bW /(float)bH, (float)0.1f, (float)100.0f);


    float angle = 0.0f;
    float sX = 0.3f;
    bool wX = true;
    float tY = 0.0f;
    while(!glfwWindowShouldClose(window)){
        
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tY += (wX) ? (0.05) : (-0.05f);

        if(tY < -0.9) wX = true;
        if(tY > 0.9) wX = false;

        glUseProgram(pid);
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                    
                    glm::mat4 model(1.0f);

                    model = glm::translate(model, glm::vec3(tY, 0, -2.0f));
                    angle += 2;
                    // model = glm::rotate(model, angle * toRad, glm::vec3(0, 1, 0));
                    model = glm::scale(model, glm::vec3(sX, sX, 0.7));
                    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));

                    glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

                    glDrawElements(GL_TRIANGLES, 20, GL_UNSIGNED_INT, nullptr);

                glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        glUseProgram(0);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

    return 0;
}
