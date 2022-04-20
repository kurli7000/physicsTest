#include "Rendering.hpp"

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
#include <sstream>
#include <iomanip>

using namespace std;

void Rendering::DrawCircle(float x, float y, float r)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        glVertex2f(x + sinf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r, y + cosf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r);
    }
    glEnd();
}

void Rendering::DrawScene(std::vector<Unit*>* units, float offset)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    
    for (auto u : *units)
    {
        DrawCircle((float)u->pos.x / (float)Vec::SCALE - 16.0f + offset,
                   (float)u->pos.y / (float)Vec::SCALE - 16.0f,
                   (float)u->radius / (float)Vec::SCALE);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    glEnd();
}

void drawString(float x, float y, string str)
{
    char arr[str.length() + 1];
    strcpy(arr, str.c_str());
    
    glRasterPos3f(x, y, 0);

    for (char* c = arr; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

void Rendering::Render(Simulation* simulation1, Simulation* simulation2)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-32, 32, -18, 16, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    DrawScene(simulation1->getUnits(), -16.0f);
    DrawScene(simulation2->getUnits(), 16.0f);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f, -16.0f);
    glVertex2f(0.0f, 16.0f);
    glVertex2f(-32.0f, -16.0f);
    glVertex2f(32.0f, -16.0f);

    glEnd();
    
    std::stringstream text;
    text << setprecision(2) << simulation1->getStableMs()<< " ms";
    drawString(-31, -17, text.str());

    std::stringstream text2;
    text2 << setprecision(2) << simulation2->getStableMs() << " ms";
    drawString(1, -17, text2.str());
    
    glutSwapBuffers();
}
