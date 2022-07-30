#include <GL/glew.h>
#include <GL/glut.h>

int main(int argc, char *argv[]){

    glutInit(&argc, argv);
    glutCreateWindow("GLEW Test");
    GLenum err = glewInit();


    if (GLEW_OK != err)
    {
        
    }

    return 0;
}
