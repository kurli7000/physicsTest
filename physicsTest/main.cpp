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

#define RADIUS 600
#define NUM_OBJECTS 200
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
    
    /*
    static clock_t timer = clock();
    int pushRed = 0;
    if (clock() > timer + 1000000)
    {
        timer = clock();
        pushRed = (rand() % 1) + 1;
    }
    */

    simulation1->Tick();
    simulation2->Tick();
    
    DrawScene(simulation1->getUnits(), -16.0f);
    DrawScene(simulation2->getUnits(), 16.0f);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f, -16.0f);
    glVertex2f(0.0f, 16.0f);
    glEnd();

    glutSwapBuffers();
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
    glutDisplayFunc(display);
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

void Setup()
{
    simulation1 = new Simulation();
    simulation2 = new Simulation();
    
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        Unit* u1 = new Unit();
        u1->m_Pos = Vec((rand() % 32) * Vec::SCALE, (rand() % 32) * Vec::SCALE);
        u1->m_Velocity = Vec((rand() % 32) - 16, (rand() % 32) - 16);
        if (i == 0)
        {
            u1->m_Mass = 1;
            u1->m_Radius = RADIUS * 3;
        }
        else
        {
            u1->m_Mass = 50;
            u1->m_Radius = RADIUS;
        }
        u1->m_OrderNumber = i + 1;
        simulation1->getUnits()->push_back(u1);
        
        Unit* u2 = new Unit();
        u2->m_Pos = u1->m_Pos;
        u2->m_Radius = u1->m_Radius;
        u2->m_Velocity = u1->m_Velocity;
        u2->m_Mass = u1->m_Mass;
        u2->m_OrderNumber = u1->m_OrderNumber;
        simulation2->getUnits()->push_back(u2);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv); 
    CreateGlutWindow();
    CreateGlutCallbacks();
    InitOpenGL();
    Setup();
    
    glutMainLoop();
    
    ExitGlut();
    return 0;
}
