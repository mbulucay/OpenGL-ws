#ifndef SHADER_P_HPP
#define SHADER_P_HPP

#include <string>
#include <map>

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

    private:

        std::map<std::string, uint32_t> uniformMap;

        std::string readShader(const char* filepath);

        uint32_t pId;
};


#endif
