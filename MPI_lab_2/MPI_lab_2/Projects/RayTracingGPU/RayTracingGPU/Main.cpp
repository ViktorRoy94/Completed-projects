
#include "GLShader.h"
#include <GL/freeglut.h>

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutCreateWindow("Work, please");
    GLenum err = glewInit();
    GLubyte* version = (GLubyte*)glGetString(GL_VERSION);
    if( err == GLEW_OK)
    {
        GLuint program = LoadShader("rayTracer.vert", "rayTracer.frag");
        glUseProgram(program);
    }
    glRecti(1,1,-1,-1);
    glutMainLoop();
    return 0;
}