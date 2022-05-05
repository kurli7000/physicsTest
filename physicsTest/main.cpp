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

void Update()
{
    static auto startTime = chrono::steady_clock::now();
    auto now = chrono::steady_clock::now();
    int ms = (int)chrono::duration_cast<chrono::milliseconds>(now - startTime).count();

    simulation1->update(ms);
    
    // rollback simulation2 randomly
    int currentTick = simulation1->getTick();
    if (currentTick > 200 && currentTick % 177 == 0 && !simulation2->isRollingBack())
    {
        int toTick = currentTick - (rand() % 200 + 30);
        simulation2->rollback(toTick);
    }
    
    simulation2->update(ms);
    
    Rendering::render(ms, simulation1, simulation2);
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
    // set up 2 simulations
    simulation1 = new Simulation("Simulation 1");
    simulation2 = new Simulation("Simulation 2");
    
    Unit::generateUnits(simulation1->getUnits(), 300);
    Unit::copyUnits(simulation1->getUnits(), simulation2->getUnits());
    
    // generate commands for the unit number 0
    for (int i = 0; i < 500; i++)
    {
        int tick = i * 50;
        Vec velocity((rand() % 700) - 350, (rand() % 700) - 350);
        Command* c1 = new Command(0, tick, velocity);
        Command* c2 = new Command(0, tick, velocity);
        simulation1->getCommands()->push_back(c1);
        simulation2->getCommands()->push_back(c2);
    }
    
    simulation1->init();
    simulation2->init();
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
