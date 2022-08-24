#ifndef SHADER_P_HPP
#define SHADER_P_HPP

#include <string>
#include <map>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class ShaderProgram{

    public:
        ShaderProgram();
        ~ShaderProgram();

        void attachShader(const char* file, uint32_t type);

        void link();
        void validate();

        void use();

        void addUniform(const std::string& uName);

        void setUniformValueFloat(const std::string& uName, const float value);
        void setUniformValueVec3(const std::string& uName, const glm::vec3 pos);
        void setUniformValueVec4(const std::string& uName, const glm::vec4 color);
    
    private:

        std::map<std::string, uint32_t> uniformMap;

        std::string readShader(const char* filepath);

        uint32_t pId;
};


#endif
