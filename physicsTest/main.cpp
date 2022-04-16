#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <math.h>
#include <vector>
#include <ctime>
#include "Vec.h"
#include "Physics.hpp"
#include "Simulation.hpp"

using namespace std;
static int win = 0;

#define NUM_SEGMENTS 20

Simulation* simulation1;
Simulation* simulation2;

void DrawCircle(float x, float y, float r)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        glVertex2f(x + sinf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r, y + cosf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r);
    }
    glEnd();
}

void DrawScene(std::vector<Unit*>* units, float offset)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    
    for (auto u : *units)
    {
        DrawCircle((float)u->m_Pos.x / (float)Vec::SCALE - 16.0f + offset,
                   (float)u->m_Pos.y / (float)Vec::SCALE - 16.0f,
                   (float)u->m_Radius / (float)Vec::SCALE);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-32, 32, -16, 16, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    DrawScene(simulation1->getUnits(), -16.0f);
    DrawScene(simulation2->getUnits(), 16.0f);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f, -16.0f);
    glVertex2f(0.0f, 16.0f);
    glEnd();

    glutSwapBuffers();
}

void Mainloop()
{
    simulation1->Tick();
    if (rand() % 6 == 0) simulation2->Tick();
    
    display();
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
