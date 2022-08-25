
#include "shaderP.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <string>

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

    std::string s =  readShader(filepath);
    const char* shader = s.c_str();
    
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


void ShaderProgram::addUniform(const std::string& uName){

    uint32_t uId = glGetUniformLocation(pId, uName.c_str());
    uniformMap.insert(std::pair<std::string, uint32_t>(uName, uId));
}


void ShaderProgram::setUniformValueFloat(const std::string& uName, const float value){
    glUniform1f(uniformMap.at(uName), value);
}

void ShaderProgram::setUniformValueVec3(const std::string& uName, const glm::vec3 pos){
    glUniform3f(uniformMap.at(uName), pos.x, pos.y, pos.z);
}

void ShaderProgram::setUniformValueVec4(const std::string& uName, const glm::vec4 color){
    glUniform4f(uniformMap.at(uName), color.r, color.g, color.b, color.a);
}

std::string ShaderProgram::readShader(const char* filepath){

    std::fstream shader(filepath);

    std::stringstream s;
    s << shader.rdbuf();

    return s.str();
}






