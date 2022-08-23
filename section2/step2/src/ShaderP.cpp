#include "../include/shaderP.hpp"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram(){
    pId = glCreateProgram();
}


ShaderProgram::~ShaderProgram(){
    glDeleteProgram(pId);
}

void ShaderProgram::link(){
    glLinkProgram(pId);

    int errorResult;
    glGetProgramiv(pId, GL_LINK_STATUS, &errorResult);
    if(!errorResult){
        char str[1000];
        int l;
        glGetProgramInfoLog(pId, sizeof(str), &l, str);
        std::cerr << "Error Linking program" << std::endl;
        return;
    }
}

void ShaderProgram::validate(){
    glValidateProgram(pId);
    
    int errorResult;
    glGetProgramiv(pId, GL_VALIDATE_STATUS, &errorResult);
    if(!errorResult){
        char str[1000];
        int l;
        glGetProgramInfoLog(pId, sizeof(str), &l, str);
        std::cerr << "Error validating program" << std::endl;
        return;
    }
}

void ShaderProgram::use(){
    glUseProgram(pId);
}

void ShaderProgram::attachShader(const char* filepath, uint32_t type){

    const char* shader = readShader(filepath).c_str();

    uint32_t shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &shader, nullptr);
    glCompileShader(shaderId);

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

    glAttachShader(pId, shaderId);

    glDeleteShader(shaderId);
}


std::string ShaderProgram::readShader(const char* filepath){

    std::fstream shader(filepath);

    std::stringstream s;
    s << shader.rdbuf();

    return s.str();
}






