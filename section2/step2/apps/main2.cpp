#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

// window size
const GLuint width = 720, height = 720;

struct ShaderSource{
    std::string VertexS;
    std::string FragmentS;
};

static ShaderSource parse_shader(const std::string& filepath){

    enum class ShaderType : int{
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;



    std::ifstream file("/home/mbulucay/Desktop/OpenGL/section2/step2/resources/shaders/packet.shader");
    std::stringstream ss[2]; // one for vertex other one fragment 0>vs 1>fs
    std::string line;

    while(getline(file, line)){

        if(line.find("#shader") != std::string::npos){  // invalid position -1

            if(line.find("VERTEX") != std::string::npos){
                type = ShaderType::VERTEX;
            }
            else if(line.find("FRAGMENT") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static uint32_t ShaderProgram(const std::string& source, const uint32_t type){

    uint32_t shaderId = glCreateShader(type);
    // const char* src = &source[0];
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    // error checking
    int errorResult;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &errorResult);
    if(errorResult == GL_FALSE){
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length);      // stack de dinamik olrak yer ayirmani sagliyor
        glGetShaderInfoLog(shaderId, length, &length, message);
        
        std::cerr << "Failed to compile " << 
            ((type == GL_VERTEX_SHADER) ? ("Vertex") : ("Fragment")) << " shader!" << std::endl;
        std::cerr << message << std::endl;
        glDeleteShader(shaderId);
    }

    return shaderId;
}

// sadece define ettigin file icerisnde cagirabiliyorusn
static uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader){

    uint32_t program = glCreateProgram();

    uint32_t vertex_shader = ShaderProgram(vertexShader, GL_VERTEX_SHADER);
    uint32_t fragment_shader = ShaderProgram(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);
    // her programda oldugu gibi programin duzgin executble olmasi  icin object dosylarini linkelemesin
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}


int main(){

    if(glfwInit() != GL_TRUE){
        std::cout << "Error glfwinit" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "Triangle", nullptr, nullptr);
    if(window == nullptr){
        std::cout << "Error window creating" << std::endl;
        glfwTerminate();
        return 2;
    }

    int bWidth, bHeight;
    glfwGetFramebufferSize(window, &bWidth, &bHeight);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::cout << "Glew init failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }
    glViewport(0, 0, bWidth, bHeight);  

    float points[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,

         0.5f,  0.5f,
        -0.5f,  0.5f,
        -0.5f, -0.5f,
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    ShaderSource ss = parse_shader("/home/mbulucay/Desktop/OpenGL/section2/step2/resources/shaders/packet.shader");

    uint32_t shader = CreateShader(ss.VertexS.c_str(), ss.FragmentS.c_str());
    glUseProgram(shader);

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
            glBindVertexArray(vao);
                glEnableVertexAttribArray(0);

                    glDrawArrays(GL_TRIANGLES, 0, 6);


                glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
