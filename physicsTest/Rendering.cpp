#include "Rendering.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include "freeglut/include/GL/freeglut.h"
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <math.h>
#include <sstream>
#include <iomanip>

using namespace std;


float Rendering::distortX(float y, float t)
{
    float s = sin(y * 0.2132 - t * 0.335 + sin(y * 0.4213 + t * 0.142));
    s *= sin(y * 0.8312 - t * 0.562);
    return fabs(pow(s, 10.0) + sin(y * 0.3 - t * 0.1) * 0.5 + 0.5);
}

void Rendering::drawCircle(float x, float y, float r, bool distort)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        float xx = x + sinf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r;
        float yy = y + cosf(i / (float)NUM_SEGMENTS * 3.141592f * 2.0f) * r;
        float xd = distortX(yy, timer / 100.0f);
        
        if (distort) glVertex2f(xx + xd, yy);
        else glVertex2f(xx, yy);
    }
    glEnd();
}

void Rendering::drawScene(Simulation* simulation, float offset)
{
    auto units = simulation->getUnits();
    float fraction = simulation->getFrameFraction(timer);
    bool rollback = simulation->isRollingBack();

    glColor3f(1.0f, 0.0f, 0.0f);
    
    for (auto u : *units)
    {
        auto pos = u->getRenderingPosition(fraction);
        drawCircle(pos.x - 16.0 + offset, pos.y - 16.0f, (float)u->radius / (float)Vec::SCALE, rollback);
        if (!rollback) glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    glEnd();
}

void drawString(float x, float y, string str, void* font = GLUT_BITMAP_9_BY_15)
{
    char arr[1024];
    strcpy_s(arr, str.c_str());
    
    glRasterPos3f(x, y, 0);

    for (char* c = arr; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void Rendering::render(int ms)
{
    timer = ms;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-32, 32, -18, 16, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawScene(simulation1, -16.0);
    drawScene(simulation2, 16.0);
    
    glColor3f(1.0, 1.0, 1.0);
    
    glBegin(GL_LINES);
    glVertex2f(0.0f, -16.0f);
    glVertex2f(0.0f, 16.0f);
    glVertex2f(-32.0f, -16.0f);
    glVertex2f(32.0f, -16.0f);
    glEnd();
    
    std::stringstream text;
    text << "tick: " << simulation1->getTick() << ", next cmd: " << simulation1->getNextCommandTick() << ", exec: "
        << setprecision(1) << std::fixed << simulation1->getStableMs()<< " ms (max " << simulation1->getMaxMs() << " ms)";
    drawString(-31, -17, text.str());

    std::stringstream text2;
    text2 << "tick: " << simulation2->getTick() << ", next cmd: " << simulation2->getNextCommandTick() << ", exec: "
        << setprecision(1) << std::fixed << simulation2->getStableMs() << " ms (max " << simulation2->getMaxMs() << " ms)";
    drawString(1, -17, text2.str());
    
    if (simulation2->isRollingBack())
    {
        glColor3f(1.0, 0.0, 0.0);
        drawString(11, 0, "OUT OF SYNC", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    
    glutSwapBuffers();
}
