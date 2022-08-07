#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

// window size
const GLuint width = 640, height = 480;

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

    // pencere yonetimi .... gibi seyler icin ilk bunu initialize etmemiz gerekiyor
    if(glfwInit() != GL_TRUE){
        std::cout << "Error glfwinit" << std::endl;
        glfwTerminate();
        return 1;
    }

    //simdi uygulama ve pencere icin temel ayarlari yapacagiz versiyon belirlemek gibi
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 

    // Versiyonlar arasi farklarda bir sorun hissederse error oluyor No backward compatibility ileriye donuk uyumluluga izin veriyoruz
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
    // glewin kullanmasai icin pencere atiyoruz bu fonksiyonla uygulamanadki pencerelerin arasinda istedigin gibi gezebilrisn ve birdenfazla window icerebilir
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
        -0.2f, -0.2f,
         0.0f,  0.5f,
         0.5f, -0.5f 
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t bufferId;
    glGenBuffers(1, &bufferId); // Gpu da yer alan bufferin id yani lokasyonu gibi bisey
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); // Bufferin ne icin nasil kullanacagimizi berlirledigimiz yer
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); // buffer icerisine datayi koyuyoruz ama 
    // nasil handle edilmesi gerektigini de son parametre olarak gonderiyoruz

    // Binding photoshop taki layer sistemi gibi hangi layer a datalari kullanarak cizmek istiyorsan o layer a bind olmalisin

    // void glEnableVertexAttribArray(	GLuint index);
    // void glDisableVertexAttribArray(	GLuint index);
    // void glEnableVertexArrayAttrib(	GLuint vaobj,
    // GLuint index);
    // void glDisableVertexArrayAttrib(	GLuint vaobj,
    // GLuint index);
    // glEnableVertexAttribArray and glEnableVertexArrayAttrib enable the generic vertex attribute array specified by index.
    glEnableVertexAttribArray(0);

    // 0-> 0 index datasi ilk oldugu icin
    // 2-> vertexin data uzunlugu
    // GL_FLOAT -> 2 de belirledigin seyin tipi 2 ne iki ne bu iki
    // GL_FALSE -> normalize edilmesine gerek yok zaten floatlar
    // sizeof(float) * 2 -> GL_SIZE stride -> vertex data uzunlugu byte cinsinde
    // (const void*)0 -> Pointerin baslangic noktasi 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glsl verison 330 kullaniyoruz 
    // code depraceted fonksiyon kullanmamiza izin yok
    // In the process of authoring computer software, its standards or documentation, deprecation is a status applied to software features to indicate that they should be avoided, typically because they have been superseded. Although deprecated features remain in the software, their use may raise warning messages recommending alternative practices, and deprecation may indicate that the feature will be removed in the future. Features are deprecated—rather than immediately removed—in order to provide backward compatibility, and give programmers who have used the feature time to bring their code into compliance with the new standard.
    // deprecation fonksiyon kullanmaktan kacinmamiz gereken fonksiyonlar bunlar ne olabilir eski surum, lack of portabiliy .... 
    std::string vertex_shader = 
        "#version 330 core\n"
        "\n"
        // atributelarin bulundugu index 0
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    const std::string vs = 
        R"glsl(
            #version 330 core

            layout(location = 0) in vec4 position;

            void main(){
                gl_Position = position;
            }
        )glsl";

    std::string fragment_shader = 
        "#version 330 core\n"
        "\n"
        // atributelarin bulundugu index 0
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.5, 0.3, 1.0);\n"
        "}\n";

    uint32_t shader = CreateShader(vs, fragment_shader);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
