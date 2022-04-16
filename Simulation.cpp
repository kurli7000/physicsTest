#include "Simulation.hpp"
#include <iostream>

using namespace std;

Simulation::Simulation() : lastTick(0)
{
    startTime = chrono::steady_clock::now();
}

int Simulation::getMs()
{
    auto now = chrono::steady_clock::now();
    return (int)chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
}

bool Simulation::Tick()
{
    int tick = getMs() / millisecondsPerTick;
    bool processed = false;
    
    while (tick > lastTick)
    {
        bool push = false;
        if (lastTick % millisecondsPerTick == 0) push = true;
        ProcessUnits(push);
        lastTick++;
        processed = true;
    }
    return processed;
}

void Simulation::ProcessUnits(bool pushRed)
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

    Physics::ResolveCollisions(units);
}


Simulation::~Simulation()
{
}

