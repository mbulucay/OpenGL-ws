#ifndef SHADER_P_HPP
#define SHADER_P_HPP

#include <string>

class ShaderProgram{

    public:
        ShaderProgram();
        ~ShaderProgram();

        void attachShader(const char* file, uint32_t type);

        void link();
        void validate();

        void use();

    private:

        std::string readShader(const char* filepath);

        uint32_t pId;
};


#endif
