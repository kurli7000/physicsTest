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

using namespace std;
static int win = 0;

#define RADIUS 600
#define NUM_OBJECTS 200
#define GRAVITY 0
#define NUM_SEGMENTS 20

std::vector<Unit*> units1;
std::vector<Unit*> units2;


void DrawCircle(float x, float y, float r)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        glVertex2f(x + sinf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r, y + cosf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r);
    }
    glEnd();
}

void ProcessUnits(std::vector<Unit*> units, int pushRed)
{
    for (auto unit : units)
    {
        unit->m_Pos += unit->m_Velocity;
        
        if (unit->m_Pos.x < unit->m_Radius)
        {
            unit->m_Pos.x = unit->m_Radius;
            unit->m_Velocity.x = -unit->m_Velocity.x;
        }
        
        if (unit->m_Pos.x > 32 * Vec::SCALE - unit->m_Radius)
        {
            unit->m_Pos.x = 32 * Vec::SCALE - unit->m_Radius;
            unit->m_Velocity.x = -unit->m_Velocity.x;
        }
        
        if (unit->m_Pos.y < unit->m_Radius)
        {
            unit->m_Pos.y = unit->m_Radius;
            unit->m_Velocity.y = -unit->m_Velocity.y;
        }
        
        if (unit->m_Pos.y > 32 * Vec::SCALE - unit->m_Radius)
        {
            unit->m_Pos.y = 32 * Vec::SCALE - unit->m_Radius;
            unit->m_Velocity.y = -unit->m_Velocity.y;
        }
        
        unit->m_Velocity.y -= GRAVITY;
    }
    
    if (pushRed)
    {
        if (pushRed == 1) units[0]->m_Velocity.x += 300;
        else units[0]->m_Velocity.x -= 300;
        units[0]->m_Velocity.y += 500;
    }

    ResolveCollisions(units);
}

void DrawScene(std::vector<Unit*> units, float offset)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    
    for (auto u : units)
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
    
    static clock_t timer = clock();
    int pushRed = 0;
    if (clock() > timer + 1000000)
    {
        timer = clock();
        pushRed = (rand() % 1) + 1;
    }

    ProcessUnits(units1, pushRed);
    DrawScene(units1, -16.0f);
    
    ProcessUnits(units2, pushRed);
    DrawScene(units2, 16.0f);
    
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

void Precalc()
{
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
        units1.push_back(u1);
        
        Unit* u2 = new Unit();
        u2->m_Pos = u1->m_Pos;
        u2->m_Radius = u1->m_Radius;
        u2->m_Velocity = u1->m_Velocity;
        u2->m_Mass = u1->m_Mass;
        u2->m_OrderNumber = u1->m_OrderNumber;
        units2.push_back(u2);
    }
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv); 
    CreateGlutWindow();
    CreateGlutCallbacks();
    InitOpenGL();
    Precalc();
    
    glutMainLoop();
    
    ExitGlut();
    return 0;
}
