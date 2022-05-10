#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "freeglut/include/GL/freeglut.h"
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <vector>
#include "Physics.hpp"
#include "Simulation.hpp"
#include "Rendering.hpp"
#include "CommandDispatcher.hpp"

using namespace std;
static int win = 0;

Simulation* simulation1;
Simulation* simulation2;
Rendering* rendering;
CommandDispatcher* dispatcher;

void Update()
{
    static auto startTime = chrono::steady_clock::now();
    auto now = chrono::steady_clock::now();
    int ms = (int)chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
    
    dispatcher->update(simulation1->getTick());
    simulation1->update(ms);
    simulation2->update(ms);
    rendering->render(ms);
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
    glutInitWindowSize (1024, 512 + 64);
    glutCreateWindow("Test");
}

void CreateGlutCallbacks()
{
    glutDisplayFunc(Update);
    glutIdleFunc(idle);
}


void ExitGlut()
{
    glutDestroyWindow(win);
    exit(0);
}

void init()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // set up 2 simulations
    simulation1 = new Simulation("Simulation 1");
    simulation2 = new Simulation("Simulation 2");
    
    Unit::generateUnits(simulation1->getUnits(), 300);
    Unit::copyUnits(simulation1->getUnits(), simulation2->getUnits());
    
    dispatcher = new CommandDispatcher(simulation1, simulation2);
    dispatcher->init();
    
    rendering = new Rendering(simulation1, simulation2);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv); 
    CreateGlutWindow();
    CreateGlutCallbacks();

    init();
    
    glutMainLoop();
    
    ExitGlut();
    return 0;
}
