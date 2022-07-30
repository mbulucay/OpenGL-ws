#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

// window size
const GLuint width = 1080, height = 720;

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

    // Versiyonlar arasi farklarda bir sorun hissederse error oluyor 
    // No backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ileriye donuk uyumluluga izin veriyoruz
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "Hello world", nullptr, nullptr);
    
    if(window == nullptr){
        std::cout << "Error window creating" << std::endl;
        glfwTerminate();
        return 2;
    }

    
    // Normalde tanimladik ama tekrardan ana pencerenin boyutlarini buradan aliyoruz
    // >???????? Neden ????? yukarda constant olarak tanimladik zaten ?????? 
    // Acaba cursor gibi bisey mi??? 
    // Galiba render edilebilen kismi aliyoruz
    int bWidth, bHeight;
    glfwGetFramebufferSize(window, &bWidth, &bHeight);


    // Simdi GLEW ayarlamalari yapcagiz os portable olmasi icin

    // glewin kullanmasai icin pencere atiyoruz
    // bu fonksiyonla uygulamanadki pencerelerin arasinda istedigin gibi gezebilrisn 
    // ve birdenfazla window icerebilir
    glfwMakeContextCurrent(window);

    // extensionlara ulasabilmek icin true atiyoruz
    // !!!!!!!! Detayli bak         a bit advance
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::cout << "Glew init failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    // notes2 q1
    glViewport(0, 0, bWidth, bHeight);  

    while(!glfwWindowShouldClose(window)){

        // handle user events
        // it uses signal i guess
        glfwPollEvents();
        /* 
            Processes all pending events. This function processes only those events that are already in the event queue and then returns immediately. 
            Processing events will cause the window and input callbacks associated with those events to be called. 
            On some platforms, a window move, resize or menu operation will cause event processing to block. 
            This is due to how event processing is designed on those platforms. You can use the [window refresh callback](window_refresh) to redraw the contents of your window when necessary during such operations. 
            Do not assume that callbacks you set will _only_ be called in response to event processing functions like this one. While it is necessary to poll for events, window systems that require GLFW to register callbacks of its own can pass events to GLFW in response to many window system function calls. 
            GLFW will pass those events on to the application callbacks before returning. Event processing is not required for joystick input to work.
        */

        // GLclampf ;typedef float GLclampf;
        glClearColor(0.2f, 1.0f, 0.5f, 0.3f);
        glClear(GL_COLOR_BUFFER_BIT);

        // there are actually 2 buffer to manage a window scene
        // one for the display user other one for editing the scene
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
