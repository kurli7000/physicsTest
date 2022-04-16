#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <vector>
#include "Physics.hpp"
#include "Simulation.hpp"
#include "Rendering.hpp"

using namespace std;
static int win = 0;

Simulation* simulation1;
Simulation* simulation2;

void Mainloop()
{
    simulation1->Tick();
    if (rand() % 6 == 0) simulation2->Tick();
    
    Rendering::Render(simulation1->getUnits(), simulation2->getUnits());
}

void idle()
{
    glutPostRedisplay();
}

void menuCallback(int option)
{
    glutPostRedisplay();
}

void CreateGlutWindow()
{
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition (10, 10);
    glutInitWindowSize (1024, 512);
    glutCreateWindow("Test");
}

void CreateGlutCallbacks()
{
    glutDisplayFunc(Mainloop);
    glutIdleFunc(idle);
}

void InitOpenGL()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ExitGlut()
{
    glutDestroyWindow(win);
    exit(0);
}

void Init()
{
    simulation1 = new Simulation();
    simulation2 = new Simulation();
    Unit::Precalc(simulation1->getUnits(), 300);
    Unit::CopyUnits(simulation1->getUnits(), simulation2->getUnits());
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv); 
    CreateGlutWindow();
    CreateGlutCallbacks();
    InitOpenGL();
    Init();
    
    glutMainLoop();
    
    ExitGlut();
    return 0;
}
