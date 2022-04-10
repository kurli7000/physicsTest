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

using namespace std;
static int win = 0;


#define RADIUS 600
#define NUM_OBJECTS 200
#define GRAVITY 1
#define NUM_SEGMENTS 20

#include "Vec.h"

class Unit
{
public:
    Unit() {};
    ~Unit() {};
    Vec m_Pos;
    Vec m_Velocity;
    int m_Radius;
    int m_Mass;
    int m_OrderNumber;
};

std::vector<Unit*> units1;
std::vector<Unit*> units2;

struct Manifold
{
    Unit* A;
    Unit* B;
    int penetration;
    Vec normal;
};

void menuCallback(int);
void DrawScene(std::vector<Unit*>, float offset, bool pushRed);

bool CreateManifold(Manifold* m)
{
    Unit* A = m->A;
    Unit* B = m->B;
    Vec normal = B->m_Pos - A->m_Pos;
    int radius = A->m_Radius + B->m_Radius;
 
    if (normal.GetLengthSquared() > radius * radius) return false;
    
    int distance = normal.GetLength();
 
    if (distance != 0)
    {
        m->penetration = (radius - distance);
        m->normal = normal * Vec::SCALE / distance;
        return true;
    }
    else
    {
        m->penetration = A->m_Radius;
        m->normal = Vec(Vec::SCALE, 0);
        return true;
    }
}


void ResolveCollision(Unit* a, Unit* b)
{
    Manifold m;
    
    m.A = a;
    m.B = b;
    
    // Adapted from https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
    
    if (CreateManifold(&m))
    {
        // relative velocity
        Vec rVelocity = b->m_Velocity - a->m_Velocity;
        
        int velAlongNormal = rVelocity.Dot(m.normal) / Vec::SCALE;

        if (velAlongNormal > 0) return; // if bodies are separating, don't resolve
        
        int inverseMassSum = (a->m_Mass + b->m_Mass);
        int j;

        if (inverseMassSum != 0)
        {
            j = -Vec::SCALE * velAlongNormal / inverseMassSum + m.penetration;
            Vec impulse = (m.normal * j / Vec::SCALE);
            a->m_Velocity -= (impulse * a->m_Mass) / Vec::SCALE;
            b->m_Velocity += (impulse * b->m_Mass) / Vec::SCALE;
        }
    }
}


void DrawCircle(float x, float y, float r)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        glVertex2f(x + sinf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r, y + cosf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r);
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
    bool pushRed = false;
    if (clock() > timer + 1000000)
    {
        timer = clock();
        pushRed = true;
    }

    DrawScene(units1, -16.0f, pushRed);
    DrawScene(units2, 16.0f, pushRed);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f, -16.0f);
    glVertex2f(0.0f, 16.0f);
    glEnd();

    glutSwapBuffers();
}


void ResolveCollisions(std::vector<Unit*> units)
{
    int processedUntil = 0;
    
    for (auto unit : units)
    {
        for (auto otherUnit : units)
        {
            if (unit != otherUnit && otherUnit->m_OrderNumber > processedUntil)
            {
                ResolveCollision(unit, otherUnit);
            }
        }
        processedUntil = unit->m_OrderNumber;
    }
}


void DrawScene(std::vector<Unit*> units, float offset, bool pushRed)
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
        units[0]->m_Velocity.x += 200;
        units[0]->m_Velocity.y += 500;
    }
        
    ResolveCollisions(units);

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

int main (int argc, char **argv)
{
    glutInit(&argc, argv); 
    CreateGlutWindow();
    CreateGlutCallbacks();
    InitOpenGL();
    
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
    
    glutMainLoop();
    
    ExitGlut();
    return 0;		
}
