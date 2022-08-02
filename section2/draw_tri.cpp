#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

// window size
const GLuint width = 640, height = 480;

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
    
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello world", nullptr, nullptr);
    
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
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f 
    };

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

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT);

        // glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
